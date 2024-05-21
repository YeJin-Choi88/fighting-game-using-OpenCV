#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>
#include <filesystem>
#include <vector>

using namespace cv;
using namespace cv::face;
using namespace std;
using namespace std::filesystem;

// ��ķ���κ��� ���� ĸó�Ͽ� �����ϴ� �Լ�
void captureFaces(const string& cascadePath, const string& outputFolder) {
    CascadeClassifier face_cascade;
    if (!face_cascade.load(cascadePath)) {
        cerr << "Error loading face cascade\n";
        return;
    }

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Error opening video capture\n";
        return;
    }

    Mat frame, gray;
    vector<Rect> faces;
    string filename;
    int img_counter = 0;

    while (capture.read(frame)) {
        if (frame.empty()) {
            cerr << "No captured frame\n";
            break;
        }
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        face_cascade.detectMultiScale(gray, faces);

        for (const auto& face : faces) {
            Mat faceROI = frame(face);
            if (!faceROI.empty()) {
                filename = outputFolder + "/face_" + to_string(img_counter++) + ".jpg";
                imwrite(filename, faceROI);
            }
        }

        imshow("Capture - Face detection", frame);
        char c = (char)waitKey(10);
        if (c == 27 || img_counter >= 100) break; // 30���� �� �Ǵ� Esc Ű�� ����
    }
    capture.release();
    destroyAllWindows();
}

// �������� �̹����� �о� �����ͼ��� �����ϴ� �Լ�
void readImages(const string& directory, vector<Mat>& images, vector<int>& labels, int label) {
    for (const auto& entry : directory_iterator(directory)) {
        if (entry.path().extension() == ".jpg") {
            Mat img = imread(entry.path().string(), IMREAD_GRAYSCALE);
            if (!img.empty()) {
                images.push_back(img);
                labels.push_back(label); // ��� �̹����� ������ �� �Ҵ�
            }
        }
    }
}

// ���� �����ϰ� �н���Ű�� �Լ�
void trainAndSaveModel(const string& dataFolder, const string& modelPath) {
    vector<Mat> images;
    vector<int> labels;
    readImages(dataFolder, images, labels, 0); // ��� �̹����� '0' �󺧷� �б�

    if (images.empty()) {
        cerr << "ERROR! ���丮�� ������ �������� �ʽ��ϴ�." << endl;
        return;
    }

    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
    model->train(images, labels);
    model->save(modelPath);
    cout << "���� �̸��� ������ ���� �����մϴ� :  " << modelPath << endl;
}

int main() {
    string cascadePath = "C:/OpenCV_4.7/build/install/etc/haarcascades/haarcascade_frontalface_alt.xml";
    string outputFolder = "C:/Users/82105/Desktop/������Ʈ/fighting-game-using-OpenCV/���ν�/Project1/pictures for readme";
    string modelName;

    // ����ڷκ��� �� �̸��� �Է¹���
    cout << "���� �̸��� �Է��ϼ���: ";
    cin >> modelName;
    string modelPath = "C:/Users/82105/Desktop/������Ʈ/fighting-game-using-OpenCV/���ν�/Project1/" + modelName + ".yml";

    create_directories(outputFolder);

    // �� ĸó
    cout << "����� �� ĸó��..." << endl;
    captureFaces(cascadePath, outputFolder);
    cout << "Face capture complete. Faces saved to " << outputFolder << endl;

    // �� �н� �� ����
    cout << "����� �� �������κ��� �н���..." << endl;
    trainAndSaveModel(outputFolder, modelPath);

    return 0;
}
