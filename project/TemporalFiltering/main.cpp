#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

class Filter {
public:
	virtual string getFileName(int n, double q) = 0;

	virtual Mat process(Mat img, int n, double q) = 0;
};

class arithmeticMeanFilter : public Filter {
public:
	string getFileName(int n, double q) {
		return "ArithmeticMeanFilter_n=" + to_string(n);
	}

	Mat process(Mat img, int n, double q) {

		Mat ansImg = img.clone();

		for (int i = 0; i < ansImg.rows; i++)
		{
			for (int j = 0; j < ansImg.cols; j++)
			{
				int temp = 0, count = 0;

				for (int x = -n; x <= n; x++)
					for (int y = -n; y <= n; y++)
						if (i + x >= 0 && x + i < ansImg.rows&&j + y >= 0 && j + y < ansImg.cols) {
							temp += img.at<uchar>(i + x, j + y);
							count++;
						}

				if (count == 0)
					throw new exception();

				ansImg.at<uchar>(i, j) = temp / count;
			}
		}

		return ansImg;
	}
};

class geometricMeanFilter : public Filter {
public:
	string getFileName(int n, double q) {
		return "GeometricMeanFilter_n=" + to_string(n);
	}

	Mat process(Mat img, int n, double q) {

		Mat ansImg = img.clone();

		for (int i = 0; i < ansImg.rows; i++)
		{
			for (int j = 0; j < ansImg.cols; j++)
			{
				double temp = 1;
				double count = 1.0 / ((2 * n + 1)*(2 * n + 1));

				for (int x = -n; x <= n; x++)
					for (int y = -n; y <= n; y++)
						if (i + x >= 0 && x + i < ansImg.rows&&j + y >= 0 && j + y < ansImg.cols) {
							temp *= pow((img.at<uchar>(i + x, j + y) + 1), count);
						}

				ansImg.at<uchar>(i, j) = min<int>((int)temp, 255);
			}
		}

		return ansImg;
	}
};

class harmonicMeanFilter : public Filter {
public:
	string getFileName(int n, double q) {
		return "HarmonicMeanFilter_n=" + to_string(n);
	}

	Mat process(Mat img, int n, double q) {
		Mat ansImg = img.clone();

		for (int i = 0; i < ansImg.rows; i++)
		{
			for (int j = 0; j < ansImg.cols; j++)
			{
				double temp = 0;
				double count = 0;

				for (int x = -n; x <= n; x++)
					for (int y = -n; y <= n; y++)
						if (i + x >= 0 && x + i < ansImg.rows&&j + y >= 0 && j + y < ansImg.cols) {
							temp += 1 / ((double)img.at<uchar>(i + x, j + y) + 1);
							count++;
						}

				if (count == 0)
					throw new exception();

				//                cout<<"temp = "<<temp<<"count = "<<count<<endl;

				ansImg.at<uchar>(i, j) = min<int>(count / temp, 255);
			}
		}

		return ansImg;
	}
};

class contraHarmonicMeanFilter : public Filter {
public:
	string getFileName(int n, double q) {
		return "ContraHarmonicMeanFilter_n=" + to_string(n) + "_q=" + to_string(q);
	}

	Mat process(Mat img, int n, double q) {
		Mat ansImg = img.clone();

		for (int i = 0; i < ansImg.rows; i++)
		{
			for (int j = 0; j < ansImg.cols; j++)
			{
				double temp1 = 0, temp2 = 0;

				for (int x = -n; x <= n; x++)
					for (int y = -n; y <= n; y++)
						if (i + x >= 0 && x + i < ansImg.rows&&j + y >= 0 && j + y < ansImg.cols) {
							temp1 += pow(img.at<uchar>(i + x, j + y) + 1, q + 1);
							temp2 += pow(img.at<uchar>(i + x, j + y) + 1, q);
						}

				if (temp2 == 0)
					throw new exception();

				ansImg.at<uchar>(i, j) = temp1 / temp2;
			}
		}

		return ansImg;
	}
};


class Procedure {
public:
	//自动3 6 9滤波
	static void go(Filter* filter, Mat img, double q, string savePath) {
		for (int i = 1; i <= 3; i++) {
			string tempPath = savePath + filter->getFileName(i * 3, q) + ".png";
			imwrite(tempPath, filter->process(img, (i * 3) / 2, q));
		}
	}
};


int main(int argc, char* argv[]) {
	if (argc < 3) {
		cout << "usage: ./TemporalFiltering.exe \"input_image_path.png\" \"output_path\"" << endl;
	}

	Mat img = imread(argv[1], IMREAD_GRAYSCALE);

	if (img.empty())
	{
		cout << "图像打开失败" << endl;
		getchar();
		return -1;
	}

	string path = argv[2];
	if (*(path.end() - 1) != '/')
		path.push_back('/');

	imshow("original", img);

	Procedure::go(new arithmeticMeanFilter(), img, 0, path);
	Procedure::go(new geometricMeanFilter(), img, 0, path);
	Procedure::go(new harmonicMeanFilter(), img, 0, path);
	Procedure::go(new contraHarmonicMeanFilter(), img, 1.5, path);
	Procedure::go(new contraHarmonicMeanFilter(), img, -1.5, path);

	cout << "completed!" << endl;

	waitKey();

	return 0;
}
