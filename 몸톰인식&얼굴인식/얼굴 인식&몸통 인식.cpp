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
    blobFromImage(frame, blob, 1 / 255.0, Size(416, 416), Scalar(0, 0, 0), true, false);
    net.setInput(blob);
    vector<Mat> outs;
    net.forward(outs, net.getUnconnectedOutLayersNames());  // ��� ��� ���̾� ��������

    float highestConfidence = 0.0;
    Rect bestBox;
    for (auto& detection : outs) {
        for (int i = 0; i < detection.rows; i++) {
            float* data = (float*)detection.data + (i * detection.cols);
            Mat scores = detection.row(i).colRange(5, detection.cols);
            Point classIdPoint;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > highestConfidence) {
                highestConfidence = confidence;
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                bestBox = Rect(left, top, width, height);
            }
        }
    }

    // ���� ���� �ŷڵ��� ���� �ٿ�� �ڽ��� �׸���
    if (highestConfidence > 0.15) {  // ������ �ּ� �ŷڵ� �Ӱ谪
        rectangle(frame, bestBox, Scalar(0, 255, 0), 2);
    }
}

int main() {
    // Haar ĳ�����̵� �� LBPH �� ���� ���
    string face_cascade_path = "C:/OpenCV_4.7/build/install/etc/haarcascades/haarcascade_frontalface_alt.xml";
    string model_path = "C:/Users/82105/Desktop/������Ʈ/fighting-game-using-OpenCV/���ν�/Project1/shin.yml";

    // DNN �� ���� ���
    string cfg = "yolov4-tiny.cfg";
    string weights = "yolov4-tiny.weights";

    CascadeClassifier face_cascade;
    if (!face_cascade.load(face_cascade_path)) {
        cerr << "Error loading face cascade\n";
        return -1;
    }

    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
    model->read(model_path);

    Net net = readNetFromDarknet(cfg, weights);
    if (net.empty()) {
        cerr << "���� �ε��� �� �����ϴ�. ���� ��θ� Ȯ���ϼ���." << endl;
        return -1;
    }
    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_OPENCL);

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
