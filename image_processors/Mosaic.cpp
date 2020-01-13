#include <opencv2/imgproc.hpp>
#include "Mosaic.h"

cv::Mat Mosaic::createMosaic() const {

    auto maxColPiece = std::max_element(pieces.begin(), pieces.end(),
                        [](const Piece& piece1, const Piece& piece2){return piece1.colIndex < piece2.colIndex;});
    auto maxRowPiece = std::max_element(pieces.begin(), pieces.end(),
                        [](const Piece& piece1, const Piece& piece2){return piece1.rowIndex < piece2.rowIndex;});
    int cols = maxColPiece->colIndex+1;
    int rows = maxRowPiece->rowIndex+1;
    int width = cols*pieces.front().image.cols;
    int height = rows*pieces.front().image.rows;
    cv::Mat mosaic = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
    for(auto& piece: pieces)
    {
        cv::Mat colorImage = piece.image;
        if(colorImage.channels() == 1)
        {
            cv::cvtColor(colorImage, colorImage, CV_GRAY2BGR);
        }

        cv::Rect roi(piece.colIndex*colorImage.cols, piece.rowIndex*colorImage.rows,
                colorImage.cols, colorImage.rows);
        colorImage.copyTo(mosaic(roi));

        int font = CV_FONT_HERSHEY_COMPLEX_SMALL;  //defining font
        int bottom;
        cv::Size textSize = cv::getTextSize(piece.name, font, 1.0, 1, &bottom);
        double scale = (static_cast<double>(piece.image.rows)/10.0) /static_cast<double>(textSize.height);
        textSize = cv::getTextSize(piece.name, font, scale, 1, &bottom);
        cv::putText(mosaic, piece.name, cv::Point(piece.colIndex*colorImage.cols, piece.rowIndex*colorImage.rows+textSize.height),
                    font, scale, cv::Scalar(255,255,0), 1);
    }
    return mosaic;
}

void Mosaic::addImage(const cv::Mat &image, int col, int row, const std::string &imageName)
{
    pieces.push_back({image, col, row, imageName});
}
void Mosaic::reset()
{
    pieces.clear();
}