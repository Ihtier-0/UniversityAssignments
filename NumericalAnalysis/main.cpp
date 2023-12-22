#include "application.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QString>

static const auto gui_type = "gui";
static const auto tests_type = "tests";

int main(int argc, char *argv[]) {
  // parser
  QCommandLineOption type_arg(
      QStringList() << "t"
                    << "type",
      QString("Application type (%1, %2)").arg(gui_type, tests_type), "type",
      "gui");

  QCommandLineParser parser;
  parser.addOption(type_arg);

  // app
  QStringList args;
  for (int i = 0; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  parser.process(args);

  const auto type = parser.value(type_arg);
  const auto gui = type == gui_type;
  const auto tests = type == tests_type;

  if (gui) {
    QApplication app(argc, argv);

    return gui_application();
  } else if (tests) {
    return tests_application();
  } else {
    return EXIT_FAILURE;
  }
}
