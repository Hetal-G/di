//
// Copyright (c) 2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <fstream>
#include <memory>
#include <iostream>

class Readable {
public:
  virtual ~Readable() = default;
  virtual int read() const = 0;
};

class Printable {
public:
  virtual ~Printable() = default;
  virtual void print(int) = 0;
};

class FileReader : public Readable {
public:
  std::istream& input;

public:
  explicit FileReader(std::istream& in) : input{in} { }
  int read() const override final { return {}; }
};

class ConsolePrinter : public Printable {
public:
  void print(int value) override final {
    std::cout << value << '\n';
  }
};

class App {
  std::shared_ptr<const Readable> reader;
  std::shared_ptr<Printable> printer;

public:
  App(std::shared_ptr<const Readable> reader,
      std::shared_ptr<Printable> printer)
    : reader{reader}, printer{printer} {}

  void run() {
    printer->print(reader->read());
  }
};

int main() {
  {
    std::fstream file{"input.txt"};
    auto reader = std::make_shared<FileReader>(file);
    auto printer = std::make_shared<ConsolePrinter>();
    App app{reader, printer};
    app.run();
  }

  {
    namespace di = boost::di;

    const auto injector = di::make_injector(
      di::bind<std::istream>.to<std::fstream>(/*"input.txt"*/),
      di::bind<class Readable>.to<FileReader>(),
      di::bind<class Printable>.to<ConsolePrinter>()
    );
    auto app = injector.create<App>();
    app.run();
  }
}
