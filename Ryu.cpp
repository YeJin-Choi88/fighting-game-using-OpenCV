#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

class Ryu {
public:
    std::vector<cv::Mat> poses; // Ryu의 포즈 이미지들
    int poseIndex; // 현재 포즈의 인덱스
    int Ryu_Score; // Ryu의 점수
    std::chrono::steady_clock::time_point lastAttackTime; // 마지막 공격 시간
    bool isFireActive; // 파이어볼 활성화 상태
    cv::Point fireballPos; // 파이어볼 위치

    Ryu() : poseIndex(0), Ryu_Score(10), isFireActive(false), fireballPos(cv::Point(600, 350))
    {
        poses.push_back(cv::imread("ryu_stand_motion.png"));
        poses.push_back(cv::imread("ryu_attack_motion.png"));
        lastAttackTime = std::chrono::steady_clock::now();
    }

    void displayPose(cv::Mat& img)
    {
        if (!poses.empty() && !poses[poseIndex].empty())
        {
            cv::Mat resizedPose, mask;
            cv::resize(poses[poseIndex], resizedPose, (poseIndex == 0) ? cv::Size(390, 800) : cv::Size(500, 800));
            cv::inRange(resizedPose, cv::Scalar(0, 0, 0), cv::Scalar(10, 10, 10), mask);
            cv::Mat invMask;
            cv::bitwise_not(mask, invMask);
            cv::Mat coloredPose;
            resizedPose.copyTo(coloredPose, invMask);
            cv::Rect roi(cv::Point(100, 100), coloredPose.size());
            cv::Mat targetROI = img(roi);
            coloredPose.copyTo(targetROI, invMask);
        }
    }

    void updatePose()
    {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastAttackTime).count();

        if (duration >= 5)
        {
            poseIndex = 1;
            lastAttackTime = now;
            isFireActive = true; // 파이어볼 활성화
        }
        else
        {
            poseIndex = 0;
            //isFireActive = false; // 파이어볼 비활성화
        }
    }

    void displayFireball(cv::Mat& img)
    {
        if (isFireActive)
        {
            cv::circle(img, fireballPos, 50, cv::Scalar(0, 0, 255), -1); // 파이어볼로 사용할 원 그리기
            fireballPos.x += 30; // 파이어볼의 이동 속도

            // 화면을 벗어났는지 확인하고 초기 위치로 리셋, 활성화 상태 변경
            if (fireballPos.x > img.cols) {
                fireballPos.x = 600; // 초기 위치로 리셋
                isFireActive = false; // 파이어볼 비활성화
            }
        }
    }

    void displayScore(cv::Mat& img)
    {
        cv::putText(img, "Ryu: " + std::to_string(Ryu_Score), cv::Point(10, 70), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 5);
    }
};

int main() {
    // 카메라 열기
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Camera could not be opened" << std::endl;
        return -1;
    }

    // Ryu 객체 생성
    Ryu ryu;
    cv::Mat frame, flipped_frame;

    // 창 생성 및 크기 설정
    cv::namedWindow("Camera", cv::WINDOW_NORMAL);
    cv::resizeWindow("Camera", 1800, 1000);

    // 게임 루프
    while (true) {
        // 프레임 캡처
        if (!cap.read(frame)) {
            std::cerr << "Error: No captured frame" << std::endl;
            break;
        }

        // 프레임 사이즈 조정 및 뒤집기
        cv::resize(frame, frame, cv::Size(1800, 1000));
        cv::flip(frame, flipped_frame, 1);

        // 포즈 업데이트
        ryu.updatePose();
        // 포즈, 파이어볼, 점수 디스플레이
        ryu.displayPose(flipped_frame);
        ryu.displayFireball(flipped_frame);
        ryu.displayScore(flipped_frame);

        // 결과 표시
        cv::imshow("Camera", flipped_frame);

        // ESC 키로 종료
        if (cv::waitKey(10) == 27) {
            break;
        }
    }

    // 자원 해제
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
