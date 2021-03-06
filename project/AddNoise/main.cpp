#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace cv;
using namespace std;

//a,b为椒和盐噪声像素的比例 范围[0,1]
//若a+b>1 优先满足椒噪声
Mat addNoise(Mat img, double a, double b) {
	Mat ans = img.clone();

	//概率精度定为20000是因为实测发现rand()函数的范围和标准库中定义的不一样
	//随机出的最大值大概只有20000左右
	//推测可能是平台环境导致的
	unsigned int A = a * 20000;
	unsigned int B = A + b * 20000;
	int t;

	srand((unsigned int)time(NULL));

	for (int x = 0; x<img.rows; x++)
		for (int y = 0; y<img.cols; y++) {
			t = std::rand()%20000;

			if (t < A)
				ans.at<uchar>(x, y) = 0;
			else if (t < B)
				ans.at<uchar>(x, y) = 255;
		}

	return ans;
}

int main(int argc,char* argv[]) {
	if (argc < 5) {
		cout << "usage: ./AddNoise.exe \"input_image_path.png\" \"output_path.png\" pepper_noise_rate salt_noise_rate" << endl;
		return -1;
	}

	Mat img = imread(argv[1], IMREAD_GRAYSCALE);

	if (img.empty())
	{
		cout << "图片打开失败" << endl;
		getchar();
		return -1;
	}

	imshow("original", img);
	
	img = addNoise(img,atof(argv[3]),atof(argv[4]));

	imwrite(argv[2],img);

	imshow("with noise",img);

	cout << "completed!" << endl;

	waitKey();

	return 0;
}
