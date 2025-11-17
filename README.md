
## Build with CMake

You can compile the project using CMake.

```
mkdir build
cd build
cmake ..
cmake --build .
```

This is the link of documentation (currently in production)  
https://yamanashi-laboratory.github.io/MarginX_docs/  


## MarginX : Margin Calculator and Circuit Optimizer from Yamanashi Lab, YNU Developed by Sho Matsuoka (in 2023) and Nakaishi Sotaro (in 2017)   
 --------------------------------------------------------------------------  
Please wait for documentation of MarginX.  
  
You must prepare 2 Files: Circuit File which is suitable for JoSIM and Judgement File which defines correct operations  
In Circuit File, you must output phase of Josephson Junction (JJ) => Example :) .print phase B1.XI10  
,and the initial of the name of element which you want to check the margin or optimize. In Judgement File, you must write switching time of JJs like below. \begining time\ \end time\ \phase\  
If the phase of targeted JJ is passed between begining time and end time, it is regarded as correct operation.  
Phases of JJ are standardized with pi   
Example :)  
B1  
100 200 1   
300 400 3   
400 500 5  
B3  
200 300 -1   
300 400 -3   
900 1000 -5  
  
...  
  
Note that you must make Judgement file name the same as Circuit file name  
without extension.  
  
--------------------------------------------------------------------------  
There are some commands below.    
-d | You can see the details of results. you can also see them in outputted text file.  
-f | the output of a figure of Margins. You can also make figures of Margins from CSV file normally outputted  
-o | change outputted file name. You must write new outputted file name without extension to the right of -o    
-j | Specify optional name of Judgement File. You must write optional judgement file name without extension to the right of -j   
Example :) ./Marginx ex02 -j ex01 -o hoge -f  
  




