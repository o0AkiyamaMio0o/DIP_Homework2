# DIP_Homework2



#### 添加噪声

输入一张需要添加噪声的图片路径和输出路径 以及添加的椒盐噪声比率

//当两噪声比率和大于1时将优先满足胡椒噪声

##### 使用方法

./AddNoise.exe 输入路径 输出路径 胡椒噪声比率 盐噪声比率

e.g. ./runable/AddNoise.exe ./testDatas/input/test2.png ./testDatas/output/test2_withNoise.png 0.05 0.05



#### 时域滤波

输入一张图片 将自动进行滤波器尺寸为3/6/9 滤波器类型为算术/几何/谐波均值/逆谐波均值滤波器的滤波 并将所有结果输出到指定路径

##### 使用方法

./TemporalFiltering.exe 输入路径 输出路径

e.g. ./runable/TemporalFiltering.exe ./testDatas/input/test3.png ./testDatas/output



#### 频域滤波

输入一张图片 将自动进行频域滤波 并输出傅里叶变换结果和相应的滤波器 并将滤波结果输出到指定路径

##### 使用方法

./FrequencyDomainFiltering.exe 输入路径 输出路径

e.g. ./runable/FrequencyDomainFiltering.exe ./testDatas/input/test1.png ./testDatas/output/test1_denoised.png