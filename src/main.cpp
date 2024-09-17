#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

cv::Mat rotateImage(const cv::Mat &image, double angle) {
    // 计算图像的中心点
    cv::Point2f center(image.cols / 2.0, image.rows / 2.0);

    // 计算旋转矩阵
    cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);

    // 计算旋转后的图像边界
    cv::Mat rotatedImage;
    cv::warpAffine(image, rotatedImage, rotationMatrix, image.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT);

    return rotatedImage;
}

cv::Mat cropImage(const cv::Mat &image, double scale) {
    // 计算裁剪区域
    int x = 0;
    int y = 0;
    int width = image.cols * scale;
    int height = image.rows * scale;

    // 裁剪图像
    cv::Rect cropRegion(x, y, width, height);
    cv::Mat croppedImage = image(cropRegion).clone();

    return croppedImage;
}

int main() {
    cv::Mat image = cv::imread("../resources/test_image.png", cv::IMREAD_COLOR);

    if (image.empty()) {
        cerr << "Image not loaded" << endl;
        return -1;
    }

    // color space
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    cv::imwrite("../resources/gray_image.png", gray_image);

    cv::Mat hsv_image;
    cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);
    cv::imwrite("../resources/hsv_image.png", hsv_image);

    // filter blur

    cv::Mat mean_blur_image;
    cv::blur(image, mean_blur_image, cv::Size(15, 15));
    cv::imwrite("../resources/mean_blur_image.png", mean_blur_image);

    cv::Mat gaussian_blur_image;
    cv::GaussianBlur(image, gaussian_blur_image, cv::Size(15, 15), 0);
    cv::imwrite("../resources/gaussian_blur_image.png", gaussian_blur_image);

    // feature extraction ,red_image
    cv::Scalar lower_red1(0, 100, 100);
    cv::Scalar high_red1(10, 255, 255);
    cv::Scalar lower_red2(160, 100, 100);
    cv::Scalar high_red2(180, 255, 255);
    cv::Mat mask1, mask2, red_mask;
    cv::inRange(hsv_image, lower_red1, high_red1, mask1);
    cv::inRange(hsv_image, lower_red2, high_red2, mask2);

    red_mask = mask1 | mask2;

    cv::Mat red_image;
    cv::bitwise_and(image, image, red_image, red_mask);
    cv::imwrite("../resources/red_image.png", red_image);


    // red_border
    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> hierarchy;
    cv::findContours(red_mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat red_border_image = image.clone();
    cv::drawContours(red_border_image, contours, -1, cv::Scalar(0, 255, 0), 2);
    cv::imwrite("../resources/red_border_image.png", red_border_image);

    cv::Mat red_bounding_image = image.clone();

    for (const auto &contour: contours) {
        cv::Rect bounding_box = cv::boundingRect(contour);
        cv::rectangle(red_bounding_image, bounding_box, cv::Scalar(0, 255, 0), 2);
    }

    cv::imwrite("../resources/red_bounding_image.png", red_bounding_image);


    cv::Mat red_area_size = image.clone();
    for (size_t i = 0; i < contours.size(); ++i) {
        double area = cv::contourArea(contours[i]);
        cv::drawContours(red_area_size, contours, static_cast<int>(i), cv::Scalar(0, 255, 0), 2);
        cv::Rect bounding_box = cv::boundingRect(contours[i]);
        std::string text = "Area: " + std::to_string(static_cast<int>(area));
        cout << "Area Size:" << area << endl;
        cv::putText(red_area_size, text, cv::Point(bounding_box.x, bounding_box.y - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
    }

    cv::imwrite("../resources/red_area_size.png", red_area_size);

// 设定高亮颜色范围
    cv::Scalar lower_color(0, 0, 200); // 调整为实际的颜色范围
    cv::Scalar upper_color(180, 255, 255);

    // 创建颜色掩码
    cv::Mat color_mask;
    cv::inRange(hsv_image, lower_color, upper_color, color_mask);

    cv::Mat extracted_image;
    image.copyTo(extracted_image, color_mask);

    cv::Mat grey_image;
    cv::cvtColor(extracted_image, grey_image, cv::COLOR_BGR2GRAY);
    cv::imwrite("../resources/grey_image.png", grey_image);

    // 二值化
    cv::Mat binary_image;
    cv::threshold(gray_image, binary_image, 127, 255, cv::THRESH_BINARY);
    cv::imwrite("../resources/binary_image.png", binary_image);

    // 膨胀
    cv::Mat dilated_image;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::dilate(binary_image, dilated_image, kernel);
    cv::imwrite("../resources/dilated_image.png", dilated_image);

    // 腐蚀
    cv::Mat eroded_image;
    cv::erode(dilated_image, eroded_image, kernel);
    cv::imwrite("../resources/eroded_image.png", eroded_image);

    // 漫水处理
    cv::Mat flood_filled_image = eroded_image.clone();
    cv::floodFill(flood_filled_image, cv::Point(0, 0), cv::Scalar(255));
    cv::imwrite("../resources/flood_filled_image.png", flood_filled_image);


    // ----------------------------------------

    // 创建一个与原始图像相同大小的副本，用于绘制
    cv::Mat drawing_image = cv::Mat::zeros(image.size(), CV_8UC3); // 用于绘制的图像，初始化为黑色

    // 绘制圆形
    cv::Point circle_center(100, 100); // 圆心
    int circle_radius = 50; // 半径
    cv::Scalar circle_color(0, 255, 0); // 绿色
    int circle_thickness = 2; // 线宽
    cv::circle(drawing_image, circle_center, circle_radius, circle_color, circle_thickness);

    // 绘制方形
    cv::Point rectangle_top_left(200, 50); // 左上角
    cv::Point rectangle_bottom_right(300, 150); // 右下角
    cv::Scalar rectangle_color(255, 0, 0); // 蓝色
    int rectangle_thickness = 2; // 线宽
    cv::rectangle(drawing_image, rectangle_top_left, rectangle_bottom_right, rectangle_color, rectangle_thickness);

    // 绘制文字
    std::string text = "OpenCV Drawing";
    cv::Point text_origin(50, 300); // 文字原点
    cv::Scalar text_color(255, 255, 255); // 白色
    double font_scale = 1; // 字体缩放
    int font_thickness = 2; // 字体厚度
    cv::putText(drawing_image, text, text_origin, cv::FONT_HERSHEY_SIMPLEX, font_scale, text_color, font_thickness);

    // 创建掩膜图像并绘制图形
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1); // 创建掩膜图像，初始化为黑色
    cv::circle(mask, circle_center, circle_radius, cv::Scalar(255), -1); // 在掩膜上绘制圆形
    cv::rectangle(mask, rectangle_top_left, rectangle_bottom_right, cv::Scalar(255), -1); // 在掩膜上绘制方形

    // 使用掩膜创建新的图像
    cv::Mat red_contour_image;
    image.copyTo(red_contour_image); // 使用原始图像作为基底

    // 绘制红色的外轮廓
    contours.clear();
    hierarchy.clear();
    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Scalar bbox_color(0, 0, 255); // 红色
    for (const auto &contour: contours) {
        cv::Rect bounding_box = cv::boundingRect(contour);
        cv::rectangle(red_contour_image, bounding_box, bbox_color, 2);
    }

    // 合并绘制结果
    cv::Mat combined_image;
    cv::addWeighted(image, 0.7, drawing_image, 0.3, 0, combined_image);

    cv::imwrite("../resources/combined_image.png", combined_image);

    // 对图像进行旋转
    double angle = 35.0; // 旋转角度
    cv::Mat rotatedImage = rotateImage(image, angle);
    cv::imwrite("../resources/rotated_image.png", rotatedImage);

    // 对图像进行裁剪
    double scale = 0.5; // 裁剪比例（裁剪为左上角 1/4）
    cv::Mat croppedImage = cropImage(image, scale);
    cv::imwrite("../resources/cropped_image.png", croppedImage);
    return 0;
}