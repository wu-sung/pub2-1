#include "rclcpp/rclcpp.hpp" // ROS2의 핵심 헤더 파일
#include "std_msgs/msg/string.hpp" // std_msgs 패키지의 String 메시지 타입
#include <memory> // 스마트 포인터를 사용하기 위한 헤더 파일
#include <chrono> // 시간 관련 기능을 사용하기 위한 헤더 파일
#include <functional> // 함수 객체를 사용하기 위한 헤더 파일

using namespace std::chrono_literals; // chrono_literals 네임스페이스 사용

// 콜백 함수: 주기적으로 메시지를 퍼블리시하는 함수
void callback(rclcpp::Node::SharedPtr node, 
              rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub)
{
    static int count; // 정적 변수: 함수가 호출될 때마다 값이 유지됨
    auto message = std_msgs::msg::String(); // 새로운 String 메시지 생성
    message.data = "Hello world! " + std::to_string(count++); // 메시지 데이터 설정
    RCLCPP_INFO(node->get_logger(), "Publish: %s", message.data.c_str()); // 로그 출력
    pub->publish(message); // 메시지 퍼블리시
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv); // ROS2 초기화
    auto node = std::make_shared<rclcpp::Node>("mynode"); // 새로운 노드 생성
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)); // QoS 프로파일 설정 (최근 10개의 메시지만 유지)
    auto pub = node->create_publisher<std_msgs::msg::String>("mytopic", qos_profile); // 퍼블리셔 생성
    std::function<void()> fn = std::bind(callback, node, pub); // 콜백 함수를 바인딩하여 함수 객체 생성
    auto timer = node->create_wall_timer(100ms, fn); // 100ms마다 콜백 함수를 호출하는 타이머 생성
    rclcpp::spin(node); // 노드를 스핀하여 콜백 함수 호출을 대기
    rclcpp::shutdown(); // ROS2 종료
    return 0; // 프로그램 종료
}