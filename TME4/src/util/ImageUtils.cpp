// ImageUtils.cpp
#include "ImageUtils.h"

namespace pr {

void findImageFiles(const std::filesystem::path& inputFolder, std::function<void(const std::filesystem::path&)> callback) {
    if (!std::filesystem::exists(inputFolder) || !std::filesystem::is_directory(inputFolder)) {
        std::cerr << "Input folder does not exist or is not a directory." << std::endl;
        return;
    }

    auto formats = QImageReader::supportedImageFormats();
    std::vector<std::string> suffixes;
    for (const auto& fmt : formats) {
        suffixes.push_back("." + fmt.toLower().toStdString());
    }

    for (const auto& entry : std::filesystem::directory_iterator(inputFolder)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (std::find(suffixes.begin(), suffixes.end(), ext) != suffixes.end()) {
                callback(entry.path());
            }
        }
    }

}

// load image using QImage API
// includes decoding from file format e.g. JPG, PNG, etc.
// builds what is essentially a bitmap in memory
// returns a QImage i such that i.isNull() on failure
QImage loadImage(const std::filesystem::path& file) {
    QImage image(QString::fromStdString(file.string()));
    if (image.isNull()) {
        std::cerr << "Could not read image: " << file << std::endl;
    }
    return image;
}

QImage resizeImage(const QImage& originalImage) {
    if (originalImage.isNull()) {
        return QImage();
    }
    return originalImage.scaled(originalImage.width() / 2, originalImage.height() / 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void saveImage(const QImage& image, const std::filesystem::path& file) {
    if (image.isNull()) {
        std::cerr << "Cannot save null image." << std::endl;
        return;
    }
    if (!image.save(QString::fromStdString(file.string()), "JPG")) {
        std::cerr << "Could not write image: " << file << std::endl;
    }
}

} // namespace pr
