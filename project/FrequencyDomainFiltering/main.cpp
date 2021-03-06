#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

Mat dft(Mat img) {
	int w = getOptimalDFTSize(img.cols);
	int h = getOptimalDFTSize(img.rows);//获取最佳尺寸

	Mat padded;
	copyMakeBorder(img, padded, 0, h - img.rows, 0, w - img.cols, BORDER_CONSTANT, Scalar::all(0));

	for (int x = 0; x < h; x++)
		for (int y = 0; y < w; y++) {
			padded.at<float>(x, y) = padded.at<float>(x, y)*pow(-1, x + y);
		}

	Mat plane[] = { Mat_<float>(padded),Mat::zeros(padded.size(),CV_32F) };
	Mat complexIm;
	merge(plane, 2, complexIm);
	dft(complexIm, complexIm);

	return complexIm;
}

Mat idft(Mat img) {
	Mat _complexim;
	img.copyTo(_complexim);
	Mat iDft[] = { Mat::zeros(img.size(),CV_32F),Mat::zeros(img.size(),CV_32F) };
	idft(_complexim, _complexim);//傅立叶逆变换
	split(_complexim, iDft);
	magnitude(iDft[0], iDft[1], iDft[0]);
	normalize(iDft[0], iDft[0], 1, 0, CV_MINMAX);
	imshow("idft", iDft[0]);

	return iDft[0];
}

//ButterWorth Filter with D0 = 20 , n = 2
void addButterWorthPattern(Mat &filter,int x, int y) {
	for (int i = 0; i < filter.rows; i++)
		for (int j = 0; j < filter.cols; j++)
			filter.at<float>(i, j) = filter.at<float>(i, j)*(1.0 - 1.0 / (1.0 + pow((pow(i - x, 2) + pow(j - y, 2)) / 400.0, 2)));
}

Mat generateFilter(Mat complexImg) {
	Mat plane[2];

	split(complexImg, plane);

	magnitude(plane[0], plane[1], plane[0]);
	normalize(plane[0], plane[0], 200.0, 0.0, NORM_MINMAX);

	Mat img = plane[0].clone();

	vector<pair<int, int>> peaks;

	double val = 0.9;

	for (int x = 0; x < img.rows; x++)
		for (int y = 0; y < img.cols; y++) {
			if (std::abs(x - img.rows / 2) < img.rows / 10 || std::abs(y - img.cols / 2) < img.cols / 10)
				continue;

			if (img.at<float>(x, y) > val) {
				peaks.push_back(pair<int, int>(x, y));

				for (int i = -50; i < 50; i++)
					for (int j = -50; j < 50; j++)
						if (x + i >= 0 && x + i < img.rows&&y + j >= 0 && y + j < img.cols)
							img.at<float>(x + i, y + j) = 0;
			}
		}

	Mat filter(img.rows, img.cols, CV_32F, Scalar::all(1));

	for (auto i : peaks)
		addButterWorthPattern(filter,i.first,i.second);

	imshow("dft", plane[0]);
	imshow("filter", filter);

	return filter;
}


int main(int argc,char* argv[]) {
	if (argc < 3) {
		cout << "usage: ./FrequencyDomainFiltering.exe \"input_image_path.png\" \"output_path.png\"" << endl;
		return -1;
	}

	Mat img = imread(argv[1], IMREAD_GRAYSCALE);

	if (img.empty())
	{
		cout << "打开失败" << endl;
		getchar();
		return -1;
	}

	img.convertTo(img, CV_32F);
	normalize(img, img, 1.0, 0.0, NORM_MINMAX);

	imshow("original", img);

	img = dft(img);

	Mat filter = generateFilter(img);

	for (int x = 0; x < img.rows; x++)
		for (int y = 0; y < img.cols; y++) {
			img.at<Vec2f>(x, y)[0] *= filter.at<float>(x, y);
			img.at<Vec2f>(x, y)[1] *= filter.at<float>(x, y);
		}

	img = idft(img);

	string savePath = argv[2];

	normalize(img,img, 255.0, 0.0, NORM_MINMAX);
	img.convertTo(img, CV_8U);

	imwrite(savePath,img);

	cout << "completed!" << endl;

	waitKey();

	return 0;
}
