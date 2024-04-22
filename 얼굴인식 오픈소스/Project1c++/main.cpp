#include<iostream>
#include<stdlib.h>
#include<stdio.h>

// OpenCV ���̺귯��
#include<opencv2\objdetect\objdetect.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include "opencv2\core.hpp"
#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include "opencv2/face.hpp"

// ���� ó���� ���� ���̺귯��
#include <fstream>
#include <sstream>

#include "FaceRec.h"

using namespace std;
using namespace cv;
using namespace cv::face;

int main()
{
	// ���� �޴� ���
	int choice;
	cout << "1. �� �ν�\n";
	cout << "2. �� �߰�\n";
	cout << "�����ϼ���: ";
	cin >> choice;

	// ���ÿ� ���� ��� ����
	switch (choice)
	{
	case 1:
		FaceRecognition(); // �� �ν� �Լ� ȣ��
		break;
	case 2:
		addFace(); // �� �߰� �Լ� ȣ��
		eigenFaceTrainer(); // EigenFace Ʈ���̳� �Լ� ȣ��
		break;
	default:
		return 0; // ���α׷� ����
	}
	return 0;
}
