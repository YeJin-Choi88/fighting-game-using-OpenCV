#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>
#include <filesystem>

using namespace cv;
using namespace cv::face;
using namespace std;

int main() {
    // Haar ĳ�����̵� �� LBPH �� ���� ���
    string face_cascade_path = "C:/OpenCV_4.7/build/install/etc/haarcascades/haarcascade_frontalface_alt.xml";
    string model_path = "C:/Users/82105/Desktop/������Ʈ/fighting-game-using-OpenCV/���ν�/Project1/face_model.yml";

    CascadeClassifier face_cascade;
    if (!face_cascade.load(face_cascade_path)) {
        cerr << "Error loading face cascade\n";
        return -1;
    }

    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
    model->read(model_path);

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Error opening video capture\n";
        return -1;
    }

    Mat frame, gray;
    vector<Rect> faces;

    while (capture.read(frame)) {
        if (frame.empty()) {
            cerr << "No captured frame\n";
            break;
        }

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        face_cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

        for (const auto& face : faces) {
            Mat faceROI = gray(face);

            int label = -1;
            double confidence = 0;
            model->predict(faceROI, label, confidence);

            string text;
            if (label == 0 && confidence < 50) {  // �ΰ��� ������ �ʿ��� �� ����
                text = "Master";
            }
            else {
                text = "Unknown";
            }

            Point pt1(face.x, face.y);
            Point pt2(face.x + face.width, face.y + face.height);
            rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2);
            putText(frame, text, Point(face.x, face.y - 5), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
        }

        imshow("Face Recognition", frame);
        char c = (char)waitKey(10);
        if (c == 27) break;  // ESC Ű�� ������ ����
    }
    capture.release();
    destroyAllWindows();

    return 0;
}
