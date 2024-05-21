#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <filesystem>
#include <vector>

using namespace cv;
using namespace cv::face;
using namespace cv::dnn;
using namespace std;
using namespace std::filesystem;

// �� �ν��� ���� �Լ�
void recognizeFaces(Mat& frame, CascadeClassifier& face_cascade, Ptr<LBPHFaceRecognizer>& model) {
    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    vector<Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    for (const auto& face : faces) {
        Mat faceROI = gray(face);
        int label = -1;
        double confidence = 0;
        model->predict(faceROI, label, confidence);

        string text = (label == 0 && confidence < 80) ? "Player1" : "Unknown";

        Point pt1(face.x, face.y);
        Point pt2(face.x + face.width, face.y + face.height);
        rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2);
        putText(frame, text, Point(face.x, face.y - 5), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
    }
}

// ��� ������ ���� �Լ�
void detectPersons(Mat& frame, Net& net) {
    Mat blob;
    blobFromImage(frame, blob, 0.007843, Size(300, 300), Scalar(127.5, 127.5, 127.5), false);
    net.setInput(blob);
    Mat detections = net.forward();
    Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++) {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > 0.1) {  // �Ӱ谪 ����
            int idx = static_cast<int>(detectionMat.at<float>(i, 1));

            // MobileNetSSD�� Ŭ���� �ε��� 15�� "person"
            if (idx == 15) {
                int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

                rectangle(frame, Point(xLeftBottom, yLeftBottom), Point(xRightTop, yRightTop), Scalar(255, 0, 0), 2); // �Ķ������� ��� ���� �ڽ� ǥ��
            }
        }
    }
}

int main() {
    // Haar ĳ�����̵� �� LBPH �� ���� ���
    string face_cascade_path = "C:/OpenCV_4.7/build/install/etc/haarcascades/haarcascade_frontalface_alt.xml";
    string model_path = "C:/Users/82105/Desktop/������Ʈ/fighting-game-using-OpenCV/���ν�/Project1/shin.yml";

    // DNN �� ���� ���
    string prototxt = "MobileNetSSD_deploy.prototxt";
    string caffemodel = "MobileNetSSD_deploy.caffemodel";

    CascadeClassifier face_cascade;
    if (!face_cascade.load(face_cascade_path)) {
        cerr << "Error loading face cascade\n";
        return -1;
    }

    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
    model->read(model_path);

    Net net = readNetFromCaffe(prototxt, caffemodel);
    if (net.empty()) {
        cerr << "���� �ε��� �� �����ϴ�. ���� ��θ� Ȯ���ϼ���." << endl;
        return -1;
    }

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Error opening video capture\n";
        return -1;
    }

    Mat frame;
    while (capture.read(frame)) {
        if (frame.empty()) {
            cerr << "No captured frame\n";
            break;
        }

        // ������ ũ�� ���� (���� ����� ����)
        Mat resizedFrame;
        resize(frame, resizedFrame, Size(800, 450)); // ���� ����� ���� ũ�� ���

        // �� �ν�
        recognizeFaces(resizedFrame, face_cascade, model);

        // ��� ����
        detectPersons(resizedFrame, net);

        // ������ ũ�� ���� (ȭ�� ��¿�)
        resize(resizedFrame, resizedFrame, Size(1600, 900));

        imshow("Face and Person Detection", resizedFrame);
        char c = (char)waitKey(10);
        if (c == 27) break;  // ESC Ű�� ������ ����
    }
    capture.release();
    destroyAllWindows();

    return 0;
}
