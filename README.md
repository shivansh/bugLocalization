# Simultaneous identification of multiple bugs

## Instructions
### Instrumentation step
- Update the shared library path appropriately.  
  For e.g. in my machine -
  ```
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$rose_dir/lib
  ```

- From [instrumentation](instrumentation), execute -
  ```
  make
  ```
  This will instrument the client-server files available at [instrumentation/testcases/CS425].

### Collecting predicate values
- Build the client-server framework from [instrumentation/testcases/CS425](instrumentation/testcases/CS425) -
  ```
  make
  ```
  **NOTE:** Ignore the generated warnings.

- Start the server from [instrumentation/testcases/CS425](instrumentation/testcases/CS425) -
  ```
  ./server_exe 127.0.0.1 6061
  ```
  **NOTE:** The IP and port are hardcoded currently. This will be fixed very soon.

- From [instrumentation](instrumentation), execute -
  ```
  ./main.sh
  ```

### Drawing inferences from the collected data
Execute the following from project root -
```
python parse_output.py
```

## References
This project is loosely based on the work done in [Statistical Debugging: Simultaneous Identification of Multiple Bugs](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.61.4631&rep=rep1&type=pdf).
