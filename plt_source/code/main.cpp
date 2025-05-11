/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"

#include <QApplication>
//#include "opencv2/opencv.hpp"
//using namespace cv;


//void opencvtest1(const QString& path)
//{

//    Mat image = imread(path.toStdString(),IMREAD_GRAYSCALE);
//    if (image.empty()) {
//        std::cerr << "Error: Unable to load image." << std::endl;
//        return;
//    }

//    // 转换为灰度图
////    Mat gray;
////    cvtColor(image, gray, COLOR_BGR2GRAY);

//    // 二值化处理
//    Mat binary;
//    threshold(image, binary, 50, 128, THRESH_BINARY);


//    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));
//    morphologyEx(binary, binary, MORPH_CLOSE, kernel); // 形态学操作
//    morphologyEx(binary, binary, MORPH_OPEN, kernel);  // 形态学操作
//    imshow("morphology", binary);

//    // 查找轮廓
//    vector<vector<cv::Point>> contours;
//    vector<Vec4i> hierarchy; // 必须添加 hierarchy 参数
//    findContours(binary, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

//    if (contours.empty()) {
//        std::cerr << "Error: No contours found." << std::endl;
//        return;
//    }

//    // 绘制轮廓
//    Mat contourOutput = Mat::zeros(image.size(), CV_8UC3);
//    for (size_t i = 0; i < contours.size(); i =i + 1) {


//        int parentIdx = hierarchy[i][3]; // 获取父轮廓索引
//        if (parentIdx != -1 && hierarchy[parentIdx][3] == -1) {
//            // 当前轮廓是第二层轮廓
//            Scalar color = Scalar(0, 255, 0); // 绿色
//            drawContours(contourOutput, contours, static_cast<int>(i), color, 2);
//        }
//        if(i == 5000) break;

//    }

//    // 显示图像
//    namedWindow("Contours", WINDOW_NORMAL);
//    imshow("Contours", contourOutput);

//    waitKey(0); // 等待用户按键

//}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow window;
    window.show();

    //opencvtest1("D:/Plt_code/test/plt_source/image/ww.png_2_0.png");
    //opencvtest1("../image/ww.png_2_0.png");

    return app.exec();
}
