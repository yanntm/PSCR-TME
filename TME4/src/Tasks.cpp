#include "Tasks.h"
#include "util/ImageUtils.h"
#include "util/thread_timer.h"
#include <thread>
#include <sstream>

namespace pr {
void treatImage(FileQueue& fileQueue, const std::filesystem::path& outputFolder) {
    pr::thread_timer timer;
    while (true) {
        std::filesystem::path file = fileQueue.pop();
        if (file == FILE_POISON) break;
        QImage original = loadImage(file);
        if (!original.isNull()) {
            QImage resized = resizeImage(original);
            saveImage(resized, outputFolder / file.filename());
        }
    }
    std::stringstream ss;
    ss << "Thread " << std::this_thread::get_id() << " (treatImage): " << timer << " ms CPU\n";
    std::cout << ss.str();
}

void reader(FileQueue& fileQueue, ImageTaskQueue& imageQueue) {
    pr::thread_timer timer;
    // Q4 : compléter la boucle de cette étape.
    (void)fileQueue;
    (void)imageQueue;
    std::stringstream ss;
    ss << "Thread " << std::this_thread::get_id() << " (reader): " << timer << " ms CPU\n";
    std::cout << ss.str();
}

void resizer(ImageTaskQueue& imageQueue, ImageTaskQueue& resizedQueue) {
    pr::thread_timer timer;
    // Q4 : compléter la boucle de cette étape.
    (void)imageQueue;
    (void)resizedQueue;
    std::stringstream ss;
    ss << "Thread " << std::this_thread::get_id() << " (resizer): " << timer << " ms CPU\n";
    std::cout << ss.str();
}

void saver(ImageTaskQueue& resizedQueue, const std::filesystem::path& outputFolder) {
    pr::thread_timer timer;
    // Q4 : compléter la boucle de cette étape.
    (void)resizedQueue;
    (void)outputFolder;
    std::stringstream ss;
    ss << "Thread " << std::this_thread::get_id() << " (saver): " << timer << " ms CPU\n";
    std::cout << ss.str();
}

} // namespace pr
