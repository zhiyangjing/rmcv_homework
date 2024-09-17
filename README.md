## OpenCV使用笔记
### OpenCV 使用笔记

本笔记涵盖了 OpenCV 在图像处理中的基本操作，包括图像的旋转、裁剪、绘制以及一些常见的图像处理步骤。以下是代码中的各个操作及其用途的详细说明。

---

#### 1. 图像旋转

**函数：** `cv::warpAffine`

**用法：**

```cpp
cv::Mat rotateImage(const cv::Mat &image, double angle) {
    cv::Point2f center(image.cols / 2.0, image.rows / 2.0);
    cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::Mat rotatedImage;
    cv::warpAffine(image, rotatedImage, rotationMatrix, image.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT);
    return rotatedImage;
}
```

**说明：**
- 计算图像的中心点作为旋转中心。
- 使用 `cv::getRotationMatrix2D` 创建旋转矩阵。
- `cv::warpAffine` 进行实际的图像旋转， `cv::INTER_CUBIC` 是插值方法， `cv::BORDER_CONSTANT` 用于边界处理。

---

#### 2. 图像裁剪

**函数：** `cv::Rect`

**用法：**

```cpp
cv::Mat cropImage(const cv::Mat &image, double scale) {
    int x = 0;
    int y = 0;
    int width = image.cols * scale;
    int height = image.rows * scale;
    cv::Rect cropRegion(x, y, width, height);
    cv::Mat croppedImage = image(cropRegion).clone();
    return croppedImage;
}
```

**说明：**
- 创建一个 `cv::Rect` 对象来定义裁剪区域。
- 使用该裁剪区域从原图像中提取子图像。

---

#### 3. 图像灰度化

**函数：** `cv::cvtColor`

**用法：**

```cpp
cv::Mat gray_image;
cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
cv::imwrite("../resources/gray_image.png", gray_image);
```

**说明：**
- `cv::cvtColor` 用于将彩色图像转换为灰度图像。
- `cv::COLOR_BGR2GRAY` 指定转换从 BGR 到灰度。

---

#### 4. 图像高斯模糊

**函数：** `cv::GaussianBlur`

**用法：**

```cpp
cv::Mat gaussian_blur_image;
cv::GaussianBlur(image, gaussian_blur_image, cv::Size(15, 15), 0);
cv::imwrite("../resources/gaussian_blur_image.png", gaussian_blur_image);
```

**说明：**
- `cv::GaussianBlur` 用于对图像应用高斯模糊。
- `cv::Size(15, 15)` 是高斯核的大小，`0` 是标准差。

---

#### 5. 图像边缘检测和轮廓绘制

**函数：** `cv::findContours`, `cv::drawContours`

**用法：**

```cpp
vector<vector<cv::Point>> contours;
vector<cv::Vec4i> hierarchy;
cv::findContours(red_mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

cv::Mat red_border_image = image.clone();
cv::drawContours(red_border_image, contours, -1, cv::Scalar(0, 255, 0), 2);
cv::imwrite("../resources/red_border_image.png", red_border_image);
```

**说明：**
- `cv::findContours` 查找图像中的轮廓。
- `cv::drawContours` 用于在图像上绘制找到的轮廓。

---

#### 6. 图像二值化

**函数：** `cv::threshold`

**用法：**

```cpp
cv::Mat binary_image;
cv::threshold(gray_image, binary_image, 127, 255, cv::THRESH_BINARY);
cv::imwrite("../resources/binary_image.png", binary_image);
```

**说明：**
- `cv::threshold` 将图像转换为二值图像。
- `127` 是阈值，`255` 是最大值， `cv::THRESH_BINARY` 是阈值化类型。

---

#### 7. 图像膨胀和腐蚀

**函数：** `cv::dilate`, `cv::erode`

**用法：**

```cpp
cv::Mat dilated_image;
cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
cv::dilate(binary_image, dilated_image, kernel);
cv::imwrite("../resources/dilated_image.png", dilated_image);

cv::Mat eroded_image;
cv::erode(dilated_image, eroded_image, kernel);
cv::imwrite("../resources/eroded_image.png", eroded_image);
```

**说明：**
- `cv::dilate` 扩展图像的白色区域。
- `cv::erode` 收缩图像的白色区域。
- `cv::getStructuringElement` 创建形态学操作所需的内核。

---

#### 8. 漫水填充

**函数：** `cv::floodFill`

**用法：**

```cpp
cv::Mat flood_filled_image = eroded_image.clone();
cv::floodFill(flood_filled_image, cv::Point(0, 0), cv::Scalar(255));
cv::imwrite("../resources/flood_filled_image.png", flood_filled_image);
```

**说明：**
- `cv::floodFill` 用于填充区域，`cv::Point(0, 0)` 是填充起点，`cv::Scalar(255)` 是填充颜色。

---

#### 9. 绘制图形（圆形、方形、文字）

**函数：** `cv::circle`, `cv::rectangle`, `cv::putText`

**用法：**

```cpp
cv::Mat drawing_image = cv::Mat::zeros(image.size(), CV_8UC3); // 用于绘制的图像

// 绘制圆形
cv::circle(drawing_image, cv::Point(100, 100), 50, cv::Scalar(0, 255, 0), 2);

// 绘制方形
cv::rectangle(drawing_image, cv::Point(200, 50), cv::Point(300, 150), cv::Scalar(255, 0, 0), 2);

// 绘制文字
cv::putText(drawing_image, "OpenCV Drawing", cv::Point(50, 300), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
```

**说明：**
- `cv::circle` 绘制圆形。
- `cv::rectangle` 绘制矩形。
- `cv::putText` 绘制文字。

---

#### 10. 解决常见错误

**错误：** `OpenCV(4.6.0) ./modules/imgproc/src/contours.cpp:195: error: (-210:Unsupported format or combination of formats)`

**解决方案：**
- `cv::findContours` 只支持单通道图像 (`CV_8UC1`) 进行轮廓检测。如果使用彩色图像或不支持的格式，将会导致该错误。
- 确保在使用 `cv::findContours` 前将图像转换为单通道格式，如灰度图或掩膜图像。

**示例修复：**

```cpp
// 将掩膜图像转换为单通道格式
cv::Mat mask_gray;
cv::cvtColor(mask, mask_gray, cv::COLOR_GRAY2BGR);
cv::findContours(mask_gray, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
```
