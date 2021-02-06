#pragma once
#include <opencv2/core/mat.hpp>

struct Piece{
    cv::Mat image;
    int colIndex;
    int rowIndex;
    std::string name;
};

class Mosaic {
public:
    void addImage(const cv::Mat& image, int col, int row, const std::string& imageName = "");
    void reset();
    cv::Mat createMosaic() const;
private:
    std::vector<Piece> pieces;
};