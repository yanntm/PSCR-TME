#pragma once

#include <QImage>
#include <QImageReader>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <functional>

namespace pr {

/**
 * @brief Finds image files in the specified folder and calls the callback for each.
 * @param inputFolder The path to the folder to search for image files.
 * @param callback A function to call for each found image file path.
 */
void findImageFiles(const std::filesystem::path& inputFolder, std::function<void(const std::filesystem::path&)> callback);

/**
 * @brief Loads an image from the specified file.
 * @param file The path to the image file.
 * @return The loaded QImage.
 */
QImage loadImage(const std::filesystem::path& file);

/**
 * @brief Resizes the given image to half of its original size in both dimensions.
 * @param originalImage The original image to resize.
 * @return The resized QImage.
 */
QImage resizeImage(const QImage& originalImage);

/**
 * @brief Saves the image to the specified file.
 * @param image The image to save.
 * @param file The path where to save the image.
 */
void saveImage(const QImage& image, const std::filesystem::path& file);

} // namespace pr
