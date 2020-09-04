# 线程局部存储
> 线程局部存储（thread-local storage, TLS）是一个使用很方便的存储线程局部数据的系统。利用TLS机制可以为进程中所有的线程关联若干个数据，各个线程通过由TLS分配的全局索引来访问与自己关联的数据。这样，每个线程都可以有线程局部的静态存储数据。

## WINDOWS
#### 必须要了解的4个函数
1. TlsAlloc
```
WINBASEAPI
DWORD
WINAPI
TlsAlloc(
    VOID
    );
```
KERNEL32 使用两个DWORDs（总共64 个位）来记录哪一个slot 是可用的、哪一个slot 已经被用。当你需要使用一个TLS slot 的时候，你就可以用这个函数将相应的TLS slot位置１。  

2. TlsSetValue
```
WINBASEAPI
BOOL
WINAPI
TlsSetValue(
    _In_ DWORD dwTlsIndex,
    _In_opt_ LPVOID lpTlsValue
    );
```
TlsSetValue 可以把数据放入先前配置到的TLS slot 中。两个参数分别是TLS slot 索引值以及欲写入的数据内容。TlsSetValue 就把你指定的数据放入64 DWORDs 所组成的数组（位于目前的thread database）的适当位置中。  

3. TlsGetValue
```
WINBASEAPI
LPVOID
WINAPI
TlsGetValue(
    _In_ DWORD dwTlsIndex
    );
```
这个函数几乎是TlsSetValue 的一面镜子，最大的差异是它取出数据而非设定数据。和TlsSetValue 一样，这个函数也是先检查TLS 索引值合法与否。如果是，TlsGetValue 就使用这个索引值找到64 DWORDs 数组（位于thread database 中）的对应数据项，并将其内容传回。  

4. TlsFree 
```
WINBASEAPI
BOOL
WINAPI
TlsFree(
    _In_ DWORD dwTlsIndex
    );
```
这个函数将TlsAlloc 和TlsSetValue 的努力全部抹消掉。TlsFree 先检验你交给它的索引值是否的确被配置过。如果是，它将对应的64 位TLS slots 位关闭。然后，为了避免那个已经不再合法的内容被使用，TlsFree 巡访进程中的每一个线程，把0 放到刚刚被释放的那个TLS slot 上头。于是呢，如果有某个TLS 索引后来又被重新配置，所有用到该索引的线程就保证会取回一个0 值，除非它们再调用TlsSetValue。

#### 步骤
1. 首先定义一些DWORD线程全局变量或函数静态变量,准备作为各个线程访问自己的TLS数组的索引变量。一个线程使用TLS时，第一步在线程内调用TlsAlloc()函数，为一个TLS数组索引变量与这个线程的TLS数组的某个槽(slot）关联起来，例如获得一个索引变量：

```
    int threadIdPionter = TLSAlloc();
```
<font size=2 color=blue>注意，此步之后，当前线程实际上访问的是这个TLS数组索引变量的线程内的拷贝版本。也就说，不同线程虽然看起来用的是同名的TLS数组索引变量，但实际上各个线程得到的可能是不同DWORD值。其意义在于，每个使用TLS的线程获得了一个DWORD类型的线程局部静态变量作为TLS数组的索引变量。C/C++原本没有直接定义线程局部静态变量的机制，所以在如此大费周折。</font>

2. 第二步，为当前线程动态分配一块内存区域（使用LocalAlloc()函数调用），然后把指向这块内存区域的指针放入TLS数组相应的槽中(使用TlsSetValue()函数调用)。
```
TlsSetValue(threadIdPionter, (LPVOID)threadId)
```
3. 第三步，在当前线程的任何函数内，都可以通过TLS数组的索引变量，使用TlsGetValue()函数得到上一步的那块内存区域的指针，然后就可以进行内存区域的读写操作了。这就实现了在一个线程内部这个范围处处可访问的变量。
```
int threadId = (int)TlsGetValue(threadIdPionter);
```
4. 最后，如果不再需要上述线程局部静态变量，要动态释放掉这块内存区域(使用LocalFree()函数)，然后从TLS数组中放弃对应的槽(使用TlsFree()函数）。
```
::TlsFree(threadIdPionter);
```


## LINUX
暂无
```
　　int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
　　int pthread_key_delete(pthread_key_t key);
　　void *pthread_getspecific(pthread_key_t key);
　　int pthread_setspecific(pthread_key_t key, const void *value);
```




