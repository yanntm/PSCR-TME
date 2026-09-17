#pragma once

#include <QImage>
#include <filesystem>
#include "BoundedBlockingQueue.h"

namespace pr {
using FileQueue = BoundedBlockingQueue<std::filesystem::path>;
inline const std::filesystem::path FILE_POISON{};

void treatImage(FileQueue& fileQueue, const std::filesystem::path& outputFolder);

struct TaskData {
    QImage image;
    std::filesystem::path filename;
};

using ImageTaskQueue = BoundedBlockingQueue<TaskData*>;
inline constexpr TaskData* TASK_POISON = nullptr;

void reader(FileQueue& fileQueue, ImageTaskQueue& imageQueue);
void resizer(ImageTaskQueue& imageQueue, ImageTaskQueue& resizedQueue);
void saver(ImageTaskQueue& resizedQueue, const std::filesystem::path& outputFolder);
} // namespace pr
