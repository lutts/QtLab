// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#include <QApplication>
#include <QMainWindow>
#include <QEvent>
#include <QLabel>
#include <QLineEdit>
#include <QCompleter>
#include <QVBoxLayout>
#include <QDebug>
#include <QFile>
#include <QTreeView>
#include <QComboBox>
#include <QStandardItemModel>

#include "./custom_update_line_edit.h"
#include "./treemodelcompleter.h"
#include "./recent_all_completer.h"
#include "./recent_all_line_edit.h"
#include "candidate_item_model.h"
#include "tree_combobox.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {
  QStringList wordList;
  wordList << QStringLiteral("one") << QStringLiteral("two")
           << QStringLiteral("three") << QStringLiteral("four");

  QWidget* widget = new QWidget(this);
  QVBoxLayout* layout = new QVBoxLayout(widget);

  layout->addWidget(new QLabel("Custom Update Completer: "));
  auto custom_update_line_edit = new LineEditWithCustomUpdateCompleter(widget);
  auto custom_update_completer = new CustomUpdateCompleter(wordList, widget);
  custom_update_completer->setCaseSensitivity(Qt::CaseInsensitive);
  custom_update_line_edit->setCompleter(custom_update_completer);

  layout->addWidget(custom_update_line_edit);
  layout->addSpacing(50);

  ///////////////////////////////
  layout->addWidget(new QLabel("TreeModelCompleter: "));

  auto root_root_item = new CandidateItem();

  auto root_item = new CandidateItem("root", "", root_root_item);

  auto foo = new CandidateItem("foo", "", root_item);
  new CandidateItem("Mark", "", foo);
  new CandidateItem("Bob", "", foo);
  new CandidateItem("Kent", "", foo);

  auto bar = new CandidateItem("bar", "", root_item);
  bar->setUnselectable();
  new CandidateItem("Ole", "", bar);
  new CandidateItem("Trond", "", bar);
  new CandidateItem("Kjetil", "", bar);
  new CandidateItem("Lasse", "", bar);

  auto baz = new CandidateItem("baz", "", root_item);
  new CandidateItem("Bengt", "", baz);
  new CandidateItem("Sven", "", baz);

  auto model = new CandidateItemModel(*root_root_item);

#if 1
  auto tree_view = new QTreeView(this);
  tree_view->header()->hide();
  tree_view->setModel(model);
  layout->addWidget(tree_view);
#endif

  auto treecompleter = new TreeModelCompleter(this);
  treecompleter->setSeparator(QLatin1String("."));
  bool case_sensitive = false;
  treecompleter->setCaseSensitivity(case_sensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
  treecompleter->setCompletionMode(QCompleter::PopupCompletion);
  treecompleter->setModel(model);

  auto treeLineEdit = new QLineEdit;
  treeLineEdit->setCompleter(treecompleter);

  layout->addWidget(treeLineEdit);
  layout->addSpacing(50);

  //////////////////////////////////////
  auto tree_combobox = new TreeComboBox(widget);
  tree_combobox->setModel(model);
  layout->addWidget(tree_combobox);
  layout->addSpacing(50);


  /////////////////////////////////////////
  layout->addWidget(new QLabel("RecentAllCompleter: "));

  static QStringList empty_word_list {
    QStringLiteral("haha"),
        QStringLiteral("hoho"),
        QStringLiteral("hehe"),
        };

  static QStringList non_empty_word_list {
    QStringLiteral("one"),
        QStringLiteral("two"),
        QStringLiteral("three"),
        QStringLiteral("four") };

  static QStringListModel empty_word_list_model(empty_word_list);
  static QStringListModel non_empty_word_list_model(non_empty_word_list);
  auto recent_all_completer = new RecentAllCompleter(empty_word_list_model,
                                                     non_empty_word_list_model);
  recent_all_completer->setCaseSensitivity(Qt::CaseInsensitive);

  auto recent_all_line_edit = new RecentAllLineEdit(widget);
  recent_all_line_edit->setCompleter(recent_all_completer);

  layout->addWidget(recent_all_line_edit);
  layout->addSpacing(20);

  auto combobox = new QComboBox(widget);
  combobox->addItems(non_empty_word_list);
  combobox->setEditable(true);
  combobox->completer()->setCompletionMode(QCompleter::PopupCompletion);
  QObject::connect(combobox, &QComboBox::editTextChanged,
                   [combobox](const QString& text){
                     qDebug() << "text = " << text
                              << "curent index = " << combobox->currentIndex();
                   });

  layout->addWidget(combobox);

  setCentralWidget(widget);
}

MainWindow::~MainWindow() {
}

#include "main.moc"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow window;
  window.show();
  return app.exec();
}
