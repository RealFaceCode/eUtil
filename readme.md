# eUtil

## Getting started

To clone the project use the following command

```bash
git clone --recursive https://github.com/RealFaceCode/eUtil.git
```

## Usage Examples

### Buffer Module

The `Array` class allows for flexible data storage and manipulation.

```cpp
#include "eutil/buffer/Array.hpp"

util::Array array(1024);
array.write("Hello, World!");
array.write(42);

std::string message = array.read<std::string>();
int number = array.read<int>();

std::cout << message << " " << number << std::endl;
```

### Filesystem Module

Perform file operations like reading, writing, and checking file existence.

```cpp
#include "eutil/filesystem/FileIOGeneric.hpp"

std::filesystem::path filePath = "example.txt";
if (!util::FileExist(filePath)) {
    util::CreateFile(filePath);
}

util::WriteFile(filePath, "Sample content");
auto content = util::LoadFile(filePath);
std::cout << content.value().data() << std::endl;
```

### Scene Management

Manage scenes and events in your application.

```cpp
#include "eutil/scene/SceneManager.hpp"

class MyScene : public util::Scene {
    void onAttach() override { std::cout << "Scene attached!" << std::endl; }
    void onUpdate() override { std::cout << "Updating scene..." << std::endl; }
    void onRender() override { std::cout << "Rendering scene..." << std::endl; }
    void onEvent(util::Event& event) override { std::cout << "Event received!" << std::endl; }
};

util::SceneManager sceneManager;
auto scene = new MyScene();
sceneManager.PushScene("MainScene", scene);
sceneManager.runScenesUpdate();
```

### Threading Module

Use the `ThreadPool` for parallel task execution.

```cpp
#include "eutil/thread/ThreadPool.hpp"

util::ThreadPool<int, int> pool([](const int& task) -> std::optional<int> {
    return task * 2;
}, 4);

pool.start();
pool.push(10);
pool.push(20);

auto results = pool.get();
if (results) {
    for (const auto& result : *results) {
        std::cout << "Result: " << result << std::endl;
    }
}
pool.stop();
```

### Time Module

Measure execution time using the `Timer` class.

```cpp
#include "eutil/time/Timer.hpp"

util::Timer timer;
timer.start();

// Simulate work
std::this_thread::sleep_for(std::chrono::seconds(1));

timer.stop();
std::cout << "Elapsed time: " << timer.elapsed() << " seconds" << std::endl;
```

### Event System

Dispatch and handle events in your application.

```cpp
#include "eutil/event/EventDispatcher.hpp"

class MyEvent : public util::Event {
public:
    std::string getName() override { return "MyEvent"; }
};

util::Event event;
util::EventDispatcher dispatcher(event);

dispatcher.dispatch<MyEvent>([](MyEvent& e) {
    std::cout << "Handling " << e.getName() << std::endl;
    return true;
});
```
