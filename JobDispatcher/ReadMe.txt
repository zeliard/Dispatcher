## WARNING!

1. you should use x64 mode due to std::atomic variable alignment
2. you should guarantee the lifetime (i.e. refcount > 0) of an object inherited from AsyncExecutable
3. be careful! when using blockable LOCKs in an async job... (which can cause deadlock.. )

