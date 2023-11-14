# HashMaker
This can be used to create specialised hash functions for your data. Lets say you have a fixed data set and you need a small but reasonable hash function you can use HashMaker to identify one. HashMaker is using an evolutionary algorithm to create random functions where each genome can hold one or more operators that operate on a fixed size hash state.

# Comparison
NOTE: I have only tested 32 bit due to memory limits the way the data is tested, but you can generate any hash length you desire if you have enough memory to test it against data.

I've tested HashMaker against a few text dictionaries and compared it against common hash algorithms such as CRC32, FNV1a, Murmur32 and it usually comes up with a better solution.

German.dic.txt with 220030 unique lines.
```
Loaded test data: F:\C++\HashMaker\data\German.dic.txt
Running FNV-1a...
-> Test Entries: 220030; Fitness: 15.99592822 ; Collisions: 0.00012726 (28) ; State: 1.00000000
Running CRC32...
-> Test Entries: 220030; Fitness: 15.99650985 ; Collisions: 0.00010908 (24) ; State: 1.00000000
Running Murmur32...
-> Test Entries: 220030; Fitness: 15.99680068 ; Collisions: 0.00009999 (22) ; State: 1.00000000
Running HashMaker<32>...
Generation: 1 ; Fitness: 13.56747841 ; Collisions: 0.08077989 (17774) ; State: 1.00000000
  -> V[0] = IN; V8[0] ROR 29; V8[2] &= IN; V8[0] ROL 11; V8[3] -= IN; V8[2] -= V8[3]; V16[2] *= 52693; V16[2] += V16[1]; V8[1] += V8[3]
Generation: 6 ; Fitness: 15.99694609 ; Collisions: 0.00009544 (21) ; State: 1.00000000
  -> V16[2] ^= 6329; V32[0] *= 3533243731; V8[0] += V8[3]; V8[0] += V8[2]; V8[0] ^= 88; V32[0] += IN; V16[2] ^= 33878; V8[2] -= V8[3]
Generation: 23 ; Fitness: 15.99709150 ; Collisions: 0.00009090 (20) ; State: 1.00000000
  -> V32[0] ^= 3312537762; V32[0] *= 3565051169; V8[0] ^= V8[3]; V8[3] += V8[0]; V8[0] += V8[2]; V16[2] += IN; V8[0] ^= V8[1]
Generation: 29 ; Fitness: 15.99723692 ; Collisions: 0.00008635 (19) ; State: 1.00000000
  -> V16[2] ^= 55871; V8[0] += IN; V32[0] *= 2049108551; V8[3] -= IN; V8[1] += V8[3]; V16[1] += IN; V8[1] -= IN
```
I left out the generations that did not improve to cut the log shorter, but as you can see FNV-1a had 28 collisions, CRC32 had 24 and Murmur32 only 22. Now after 29 generations generating 32 bit hashes it came up with a solution that would only have 19 collisions in total.

If we look closer at the solution
```
V16[2] ^= 55871; V8[0] += IN; V32[0] *= 2049108551; V8[3] -= IN; V8[1] += V8[3]; V16[1] += IN; V8[1] -= IN
```
you could actually create a simple C version of it where V8 means access 8 bits, V16 accesses 16 etc, at the given offset of the hash.

We can also do even better, I have used 'openwall.net-all.txt' which is full of passwords to see how much better it can do.
```
Loaded test data: F:\C++\HashMaker\data\openwall.net-all.txt
Running FNV-1a...
-> Test Entries: 3721255; Fitness: 15.98592767 ; Collisions: 0.00043991 (1637) ; State: 1.00000000
Running CRC32...
-> Test Entries: 3721255; Fitness: 15.98590189 ; Collisions: 0.00044071 (1640) ; State: 1.00000000
Running Murmur32...
-> Test Entries: 3721255; Fitness: 15.98636595 ; Collisions: 0.00042620 (1586) ; State: 1.00000000
Running HashMaker<32>...
Generation: 1 ; Fitness: 15.61538087 ; Collisions: 0.01212924 (45136) ; State: 1.00000000
  -> V16[2] -= V16[1]; V8[1] ^= IN; V8[1] ^= IN; V16[1] ^= 31552; V16[2] = ~V16[2]; V8[0] += V8[3]; V[0] = IN; V16[0] ^= IN; V16[2] ^= V16[0]; V16[0] = ~V16[0]; V32[0] -= IN; V8[0] += V8[1]; V8[0] += IN; V16[1] -= IN; V8[2] ^= IN
Generation: 7 ; Fitness: 15.98658079 ; Collisions: 0.00041948 (1561) ; State: 1.00000000
  -> V32[0] *= 2221766161; V8[2] += V8[0]; V16[2] += IN; V32[0] = ~V32[0]; V16[0] *= 41377; V8[0] += IN
Generation: 16 ; Fitness: 15.98664955 ; Collisions: 0.00041733 (1553) ; State: 1.00000000
  -> V16[0] ^= IN; V16[2] ^= 54572; V16[2] = ~V16[2]; V32[0] *= 3442906423; V8[2] ^= V8[3]; V8[1] += IN
Generation: 17 ; Fitness: 15.98673548 ; Collisions: 0.00041465 (1543) ; State: 1.00000000
  -> V8[3] ^= 28; V8[3] -= V8[1]; V16[0] ^= 9311; V16[0] -= V16[1]; V32[0] *= 805194117; V8[1] += IN
Generation: 27 ; Fitness: 15.98673548 ; Collisions: 0.00041465 (1543) ; State: 1.00000000
  -> V8[3] ^= 28; V8[3] -= V8[1]; V16[0] ^= 9311; V16[0] -= V16[1]; V32[0] *= 805194117; V8[1] += IN
Generation: 28 ; Fitness: 15.98689017 ; Collisions: 0.00040981 (1525) ; State: 1.00000000
  -> V16[2] -= IN; V32[0] *= 1947222885; V32[0] += IN
Generation: 29 ; Fitness: 15.98715659 ; Collisions: 0.00040148 (1494) ; State: 1.00000000
  -> V32[0] *= 1879633923; V32[0] += IN
```
As the log suggests, FNV-1a has 1637 collisions, CRC32 has 1640 and Murmur32 has 1586 which seems reasonable among 3721255 entries. But if we look at generation 29 we can actually see that it only has 1494 collisions, so we managed reduce 31 collisions with a very simple hash.

If we were to make a C version of the hash it would look something like this:
```
typedef uint8_t[4] Hash_t;

Hash_t compute(const uint8_t* input, size_t len)
{
	Hash_t hash = {0};
	for(size_t i = 0; i < len; i++)
	{
		*(uint32_t*)(hash) *= 1879633923;
		*(uint32_t*)(hash) += input[i];
	}
	return hash;
}
```
Which turns out is a pretty decent hash, you could probably optimize it to feed it directly 32 bit chunks instead of doing it bytewise but that may cause different results, so there is even room for solutions afterwards.

# Performance
While reasonable it could do better, it still takes up to an hour to find a decent hash at a larger data set. One major improvement could be JIT generation of all the operators, something like asmjit could be used to create direct functions out of each genome which should eliminate all the virtual calls and can directly use registers to operate on eliminating most memory operations currently required.

# Author
ZehMatt

# Disclaimer
We hereby emphasize, that the hacking related stuff on
[nullsecurity.net](http://nullsecurity.net) is only for education purposes.
We are not responsible for any damages. You are responsible for your own
actions.
