#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>
#include <filesystem>
#include <vector>

using namespace cv;
using namespace cv::face;
using namespace std;
using namespace std::filesystem;

// ���� �� ��� �̹����� �о �����ͼ��� �����ϴ� �Լ�
void readImages(const string& directory, vector<Mat>& images, vector<int>& labels, int label) {
    for (const auto& entry : directory_iterator(directory)) {
        if (entry.path().extension() == ".jpg") {
            Mat img = imread(entry.path().string(), IMREAD_GRAYSCALE);
            if (!img.empty()) {
                images.push_back(img);
                labels.push_back(label); // ��� �̹����� ���� �� �ο�
            }
        }
    }
}

int main() {
    string dataFolder = "C:/Users/82105/Desktop/������Ʈ/fighting-game-using-OpenCV/���ν�/Project1/pictures for readme";
    string modelPath = "C:/Users/82105/Desktop/������Ʈ/fighting-game-using-OpenCV/���ν�/Project1/face_model.yml";

    vector<Mat> images;
    vector<int> labels;

    // ������ �������� �̹����� �� �б�
    readImages(dataFolder, images, labels, 0); // 0�� ��

    if (images.empty()) {
        cerr << "No images found in the directory." << endl;
        return 1;
    }

    // �� �ν� �� ����
    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();

    // �̹����� �� Ʈ���̴�
    cout << "Training the face recognizer..." << endl;
    model->train(images, labels);

    // ���� ���Ϸ� ����
    model->save(modelPath);
    cout << "Model trained and saved at " << modelPath << endl;

    return 0;
}
