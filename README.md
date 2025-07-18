# multithread-file-compression-tool

COMPANY: CODTECH IT SOLUTIONS PVT.LTD
*NAME: Vinay Kumar Hembram
INTERN ID:CT04DG1922 
DOMAIN: C++ LANGUAGE
DURATION: 4 WEEKS
MENTOR: NEELA SANTOSH KUMAR
paragraph: 
The provided C++ program is a multithreaded file compression tool that uses the Zlib library to compress a file efficiently by leveraging parallel processing. The core idea behind this implementation is to divide a large file into equal chunks and compress each chunk in a separate thread, thereby improving performance by taking advantage of multiple CPU cores. The program begins by defining a constant `numThreads` set to 4, which dictates the number of threads that will be used for compression. It declares two global vectors: `compressedChunks` to store the compressed data for each chunk, and `threadTimes` to store the time taken by each thread.

The `compressChunk` function is responsible for compressing an individual chunk using Zlib's `compress` function. It takes the input chunk, output buffer reference, and thread index as parameters. Inside the function, it calculates the required buffer size using `compressBound`, resizes the output buffer, performs the compression, and then records the time taken. If compression fails, it clears the output buffer and logs an error message. This function also prints the time each thread takes to compress its assigned chunk, which helps in performance monitoring.

In the `main` function, the program first opens an input file named `"input.txt"` in binary mode and reads its contents into a vector. If the file cannot be opened, it displays an error and exits. The file size is calculated and divided by the number of threads to determine the chunk size. A loop then splits the file into multiple chunks, storing them in a 2D vector. Each chunk is a portion of the file data that will be independently compressed.

After preparing the data, the program initiates multithreaded compression. It creates a thread for each chunk, passing the corresponding input and output buffers along with the thread index. Once all threads are created, it waits for each one to finish using the `join()` method. The total multithreaded compression time is recorded using `high_resolution_clock`.

Following the multithreaded compression, the program writes the compressed data to an output file named `"compressed.bin"`. It includes each chunk’s size (as a 4-byte unsigned integer) followed by the chunk’s data. This format ensures that during decompression, the exact size of each chunk is known.

To compare the performance, the program also runs a single-threaded compression on the same chunks sequentially and records the total time. It then calculates the speedup gained from using multithreading by dividing the single-threaded duration by the multithreaded duration. The final output includes metrics like multithreaded and single-threaded times, performance gain (e.g., "2.5x Faster Using Threads"), and the compressed output file size in megabytes.

This program effectively demonstrates how multithreading can significantly enhance the performance of CPU-intensive tasks like file compression. It’s a practical implementation of concurrent programming principles and can be extended further with additional features like decompression, dynamic thread allocation, or support for different compression algorithms.

Output:
<img width="1920" height="909" alt="Image" src="https://github.com/user-attachments/assets/d6317f22-a58d-4309-bbbc-c97e47b9acbb" />
