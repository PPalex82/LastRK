#include "mainwindow.h"
#include <QtWidgets>
#include <QFileDialog>
#include <QMessageBox>
#include <QDataStream>
#include <QTextStream>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->table->setColumnCount(7);
    QStringList headers;
    headers << "Название" << "Жанр" << "Автор" << "Издательство" << "Количество" << "Цена" << "Путь к фотографии";
    ui->table->setHorizontalHeaderLabels(headers);
    ui->table->setAlternatingRowColors(true);
    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table->horizontalHeader()->setStretchLastSection(true);

    ui->statusLabel->setText("Готово к работе");
    ui->photoLabel->setText("Нет фотографии");

    QMenu *fileMenu = menuBar()->addMenu("Файл");

    QAction *openTextAction = new QAction("Открыть текстовый файл", this);
    QAction *saveTextAction = new QAction("Сохранить текстовый файл", this);
    QAction *openBinaryAction = new QAction("Открыть бинарный файл", this);
    QAction *saveBinaryAction = new QAction("Сохранить бинарный файл", this);
    QAction *exitAction = new QAction("Выход", this);

    fileMenu->addAction(openTextAction);
    fileMenu->addAction(saveTextAction);
    fileMenu->addSeparator();
    fileMenu->addAction(openBinaryAction);
    fileMenu->addAction(saveBinaryAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu *editMenu = menuBar()->addMenu("Правка");

    QAction *addBookAction = new QAction("Добавить книгу", this);
    QAction *deleteAction = new QAction("Удалить выбранную запись", this);
    QAction *selectPhotoAction = new QAction("Выбрать фотографию", this);

    editMenu->addAction(addBookAction);
    editMenu->addAction(deleteAction);
    editMenu->addAction(selectPhotoAction);

    QMenu *searchMenu = menuBar()->addMenu("Поиск");

    QAction *findByNameAction = new QAction("Найти книгу", this);
    searchMenu->addAction(findByNameAction);

    QMenu *sortMenu = menuBar()->addMenu("Сортировка");

    QAction *sortAscAction = new QAction("По автору А...Я", this);
    QAction *sortDescAction = new QAction("По автору Я...А", this);

    sortMenu->addAction(sortAscAction);
    sortMenu->addAction(sortDescAction);

    connect(openTextAction, &QAction::triggered, this, &MainWindow::on_openTextFile_triggered);
    connect(saveTextAction, &QAction::triggered, this, &MainWindow::on_saveTextFile_triggered);
    connect(openBinaryAction, &QAction::triggered, this, &MainWindow::on_openBinaryFile_triggered);
    connect(saveBinaryAction, &QAction::triggered, this, &MainWindow::on_saveBinaryFile_triggered);
    connect(exitAction, &QAction::triggered, this, &MainWindow::on_exit_triggered);

    connect(addBookAction, &QAction::triggered, this, &MainWindow::on_addBook_triggered);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::on_deleteSelected_triggered);
    connect(selectPhotoAction, &QAction::triggered, this, &MainWindow::on_selectPhoto_triggered);

    connect(findByNameAction, &QAction::triggered, this, &MainWindow::on_findByName_triggered);

    connect(sortAscAction, &QAction::triggered, this, &MainWindow::on_sortByAuthorAsc_triggered);
    connect(sortDescAction, &QAction::triggered, this, &MainWindow::on_sortByAuthorDesc_triggered);

    connect(ui->table, &QTableWidget::currentCellChanged, this, &MainWindow::on_table_currentCellChanged);

    ui->table->insertRow(0);
    ui->table->setItem(0, 0, new QTableWidgetItem("Мастер и Маргарита"));
    ui->table->setItem(0, 1, new QTableWidgetItem("Роман"));
    ui->table->setItem(0, 2, new QTableWidgetItem("Михаил Булгаков"));
    ui->table->setItem(0, 3, new QTableWidgetItem("АСТ"));
    ui->table->setItem(0, 4, new QTableWidgetItem("5"));
    ui->table->setItem(0, 5, new QTableWidgetItem("450"));
    ui->table->setItem(0, 6, new QTableWidgetItem(""));

    ui->table->insertRow(1);
    ui->table->setItem(1, 0, new QTableWidgetItem("Преступление и наказание"));
    ui->table->setItem(1, 1, new QTableWidgetItem("Роман"));
    ui->table->setItem(1, 2, new QTableWidgetItem("Фёдор Достоевский"));
    ui->table->setItem(1, 3, new QTableWidgetItem("Эксмо"));
    ui->table->setItem(1, 4, new QTableWidgetItem("3"));
    ui->table->setItem(1, 5, new QTableWidgetItem("520"));
    ui->table->setItem(1, 6, new QTableWidgetItem(""));

    ui->table->insertRow(2);
    ui->table->setItem(2, 0, new QTableWidgetItem("Война и мир"));
    ui->table->setItem(2, 1, new QTableWidgetItem("Роман-эпопея"));
    ui->table->setItem(2, 2, new QTableWidgetItem("Лев Толстой"));
    ui->table->setItem(2, 3, new QTableWidgetItem("Наука"));
    ui->table->setItem(2, 4, new QTableWidgetItem("10"));
    ui->table->setItem(2, 5, new QTableWidgetItem("680"));
    ui->table->setItem(2, 6, new QTableWidgetItem(""));

    ui->table->insertRow(3);
    ui->table->setItem(3, 0, new QTableWidgetItem("Евгений Онегин"));
    ui->table->setItem(3, 1, new QTableWidgetItem("Роман в стихах"));
    ui->table->setItem(3, 2, new QTableWidgetItem("Александр Пушкин"));
    ui->table->setItem(3, 3, new QTableWidgetItem("Детская литература"));
    ui->table->setItem(3, 4, new QTableWidgetItem("1"));
    ui->table->setItem(3, 5, new QTableWidgetItem("350"));
    ui->table->setItem(3, 6, new QTableWidgetItem(""));
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::BookData MainWindow::getBookDataFromRow(int row)
{
    BookData data;
    if (row < 0 || row >= ui->table->rowCount()) return data;

    data.name = ui->table->item(row, 0) ? ui->table->item(row, 0)->text() : "";
    data.genre = ui->table->item(row, 1) ? ui->table->item(row, 1)->text() : "";
    data.author = ui->table->item(row, 2) ? ui->table->item(row, 2)->text() : "";
    data.publisher = ui->table->item(row, 3) ? ui->table->item(row, 3)->text() : "";
    data.quantity = ui->table->item(row, 4) ? ui->table->item(row, 4)->text().toInt() : 0;
    data.price = ui->table->item(row, 5) ? ui->table->item(row, 5)->text().toDouble() : 0;
    data.photoPath = ui->table->item(row, 6) ? ui->table->item(row, 6)->text() : "";

    return data;
}

int MainWindow::findBookByFields(const QString& name, const QString& genre,
                                 const QString& publisher, const QString& author)
{
    for (int row = 0; row < ui->table->rowCount(); row++) {
        QString rowName = ui->table->item(row, 0) ? ui->table->item(row, 0)->text() : "";
        QString rowGenre = ui->table->item(row, 1) ? ui->table->item(row, 1)->text() : "";
        QString rowAuthor = ui->table->item(row, 2) ? ui->table->item(row, 2)->text() : "";
        QString rowPublisher = ui->table->item(row, 3) ? ui->table->item(row, 3)->text() : "";

        bool nameMatch = name.isEmpty() || rowName.contains(name, Qt::CaseInsensitive);
        bool genreMatch = genre.isEmpty() || rowGenre.contains(genre, Qt::CaseInsensitive);
        bool authorMatch = author.isEmpty() || rowAuthor.contains(author, Qt::CaseInsensitive);
        bool publisherMatch = publisher.isEmpty() || rowPublisher.contains(publisher, Qt::CaseInsensitive);

        if (nameMatch && genreMatch && authorMatch && publisherMatch) {
            return row;
        }
    }
    return -1;
}

void MainWindow::updateTableAfterPurchase(int row)
{
    if (row < 0 || row >= ui->table->rowCount()) return;

    int currentQuantity = ui->table->item(row, 4)->text().toInt();
    currentQuantity--;

    if (currentQuantity <= 0) {
        QString bookName = ui->table->item(row, 0)->text();
        ui->table->removeRow(row);
        ui->statusLabel->setText("Книга \"" + bookName + "\" продана. Запись удалена из каталога.");
        loadPhotoForRow(ui->table->currentRow());
    } else {
        ui->table->item(row, 4)->setText(QString::number(currentQuantity));
        ui->statusLabel->setText("Куплена книга \"" + ui->table->item(row, 0)->text() +
                                 "\". Осталось: " + QString::number(currentQuantity));
    }
}

void MainWindow::loadPhotoForRow(int row)
{
    if (row < 0 || row >= ui->table->rowCount()) {
        ui->photoLabel->setText("Нет фотографии");
        ui->photoLabel->setPixmap(QPixmap());
        return;
    }

    QString photoPath = ui->table->item(row, 6) ? ui->table->item(row, 6)->text() : "";

    if (photoPath.isEmpty() || !QFile::exists(photoPath)) {
        ui->photoLabel->setText("Нет фотографии");
        ui->photoLabel->setPixmap(QPixmap());
    } else {
        QPixmap pixmap(photoPath);
        if (!pixmap.isNull()) {
            QPixmap scaledPixmap = pixmap.scaled(ui->photoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->photoLabel->setPixmap(scaledPixmap);
            ui->photoLabel->setText("");
        } else {
            ui->photoLabel->setText("Ошибка загрузки");
            ui->photoLabel->setPixmap(QPixmap());
        }
    }
}

void MainWindow::on_table_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    loadPhotoForRow(currentRow);
}

void MainWindow::on_openTextFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть текстовый файл", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QTextStream in(&file);
    ui->table->setRowCount(0);
    int row = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");
        if (parts.size() >= 7) {
            ui->table->insertRow(row);
            ui->table->setItem(row, 0, new QTableWidgetItem(parts[0]));
            ui->table->setItem(row, 1, new QTableWidgetItem(parts[1]));
            ui->table->setItem(row, 2, new QTableWidgetItem(parts[2]));
            ui->table->setItem(row, 3, new QTableWidgetItem(parts[3]));
            ui->table->setItem(row, 4, new QTableWidgetItem(parts[4]));
            ui->table->setItem(row, 5, new QTableWidgetItem(parts[5]));
            ui->table->setItem(row, 6, new QTableWidgetItem(parts[6]));
            row++;
        }
    }

    file.close();
    ui->statusLabel->setText("Файл загружен: " + fileName);
    loadPhotoForRow(ui->table->currentRow());
}

void MainWindow::on_saveTextFile_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить текстовый файл", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < ui->table->rowCount(); i++) {
        QString name = ui->table->item(i, 0) ? ui->table->item(i, 0)->text() : "";
        QString genre = ui->table->item(i, 1) ? ui->table->item(i, 1)->text() : "";
        QString author = ui->table->item(i, 2) ? ui->table->item(i, 2)->text() : "";
        QString publisher = ui->table->item(i, 3) ? ui->table->item(i, 3)->text() : "";
        QString quantity = ui->table->item(i, 4) ? ui->table->item(i, 4)->text() : "";
        QString price = ui->table->item(i, 5) ? ui->table->item(i, 5)->text() : "";
        QString path = ui->table->item(i, 6) ? ui->table->item(i, 6)->text() : "";
        out << name << "|" << genre << "|" << author << "|" << publisher << "|" << quantity << "|" << price << "|" << path << "\n";
    }

    file.close();
    ui->statusLabel->setText("Файл сохранен: " + fileName);
}

void MainWindow::on_openBinaryFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть бинарный файл", "", "Binary Files (*.dat)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QDataStream in(&file);
    int rowCount;
    in >> rowCount;

    ui->table->setRowCount(0);
    for (int i = 0; i < rowCount; i++) {
        QString name, genre, author, publisher, quantity, price, path;
        in >> name >> genre >> author >> publisher >> quantity >> price >> path;
        ui->table->insertRow(i);
        ui->table->setItem(i, 0, new QTableWidgetItem(name));
        ui->table->setItem(i, 1, new QTableWidgetItem(genre));
        ui->table->setItem(i, 2, new QTableWidgetItem(author));
        ui->table->setItem(i, 3, new QTableWidgetItem(publisher));
        ui->table->setItem(i, 4, new QTableWidgetItem(quantity));
        ui->table->setItem(i, 5, new QTableWidgetItem(price));
        ui->table->setItem(i, 6, new QTableWidgetItem(path));
    }

    file.close();
    ui->statusLabel->setText("Бинарный файл загружен: " + fileName);
    loadPhotoForRow(ui->table->currentRow());
}

void MainWindow::on_saveBinaryFile_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить бинарный файл", "", "Binary Files (*.dat)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }

    QDataStream out(&file);
    out << ui->table->rowCount();

    for (int i = 0; i < ui->table->rowCount(); i++) {
        QString name = ui->table->item(i, 0) ? ui->table->item(i, 0)->text() : "";
        QString genre = ui->table->item(i, 1) ? ui->table->item(i, 1)->text() : "";
        QString author = ui->table->item(i, 2) ? ui->table->item(i, 2)->text() : "";
        QString publisher = ui->table->item(i, 3) ? ui->table->item(i, 3)->text() : "";
        QString quantity = ui->table->item(i, 4) ? ui->table->item(i, 4)->text() : "";
        QString price = ui->table->item(i, 5) ? ui->table->item(i, 5)->text() : "";
        QString path = ui->table->item(i, 6) ? ui->table->item(i, 6)->text() : "";
        out << name << genre << author << publisher << quantity << price << path;
    }

    file.close();
    ui->statusLabel->setText("Бинарный файл сохранен: " + fileName);
}

void MainWindow::on_exit_triggered()
{
    close();
}

void MainWindow::on_addBook_triggered()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить книгу");
    dialog.resize(400, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLineEdit *nameEdit = new QLineEdit(&dialog);
    nameEdit->setPlaceholderText("Название");
    QLineEdit *genreEdit = new QLineEdit(&dialog);
    genreEdit->setPlaceholderText("Жанр");
    QLineEdit *authorEdit = new QLineEdit(&dialog);
    authorEdit->setPlaceholderText("Автор");
    QLineEdit *publisherEdit = new QLineEdit(&dialog);
    publisherEdit->setPlaceholderText("Издательство");
    QLineEdit *quantityEdit = new QLineEdit(&dialog);
    quantityEdit->setPlaceholderText("Количество");
    QLineEdit *priceEdit = new QLineEdit(&dialog);
    priceEdit->setPlaceholderText("Цена");
    QLineEdit *pathEdit = new QLineEdit(&dialog);
    pathEdit->setPlaceholderText("Путь к фотографии");

    QPushButton *selectPhotoBtn = new QPushButton("Выбрать фото", &dialog);
    QPushButton *okBtn = new QPushButton("OK", &dialog);
    QPushButton *cancelBtn = new QPushButton("Отмена", &dialog);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(selectPhotoBtn);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addWidget(nameEdit);
    layout->addWidget(genreEdit);
    layout->addWidget(authorEdit);
    layout->addWidget(publisherEdit);
    layout->addWidget(quantityEdit);
    layout->addWidget(priceEdit);
    layout->addWidget(pathEdit);
    layout->addLayout(btnLayout);

    connect(selectPhotoBtn, &QPushButton::clicked, [&]() {
        QString photoPath = QFileDialog::getOpenFileName(&dialog, "Выбрать фото", "", "Images (*.png *.jpg *.bmp)");
        if (!photoPath.isEmpty()) {
            pathEdit->setText(photoPath);
        }
    });

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int row = ui->table->rowCount();
        ui->table->insertRow(row);
        ui->table->setItem(row, 0, new QTableWidgetItem(nameEdit->text()));
        ui->table->setItem(row, 1, new QTableWidgetItem(genreEdit->text()));
        ui->table->setItem(row, 2, new QTableWidgetItem(authorEdit->text()));
        ui->table->setItem(row, 3, new QTableWidgetItem(publisherEdit->text()));
        ui->table->setItem(row, 4, new QTableWidgetItem(quantityEdit->text()));
        ui->table->setItem(row, 5, new QTableWidgetItem(priceEdit->text()));
        ui->table->setItem(row, 6, new QTableWidgetItem(pathEdit->text()));
        ui->statusLabel->setText("Книга добавлена: " + nameEdit->text());
    }
}

void MainWindow::on_deleteSelected_triggered()
{
    int currentRow = ui->table->currentRow();
    if (currentRow >= 0) {
        QString name = ui->table->item(currentRow, 0)->text();
        ui->table->removeRow(currentRow);
        ui->statusLabel->setText("Удалена запись: " + name);
        loadPhotoForRow(ui->table->currentRow());
    } else {
        QMessageBox::information(this, "Информация", "Пожалуйста, выберите запись для удаления");
    }
}

void MainWindow::on_selectPhoto_triggered()
{
    QString photoPath = QFileDialog::getOpenFileName(this, "Выбрать фотографию", "", "Images (*.png *.jpg *.bmp)");
    if (!photoPath.isEmpty()) {
        int currentRow = ui->table->currentRow();
        if (currentRow >= 0) {
            ui->table->setItem(currentRow, 6, new QTableWidgetItem(photoPath));
            ui->statusLabel->setText("Фотография выбрана для записи " + QString::number(currentRow + 1));
            loadPhotoForRow(currentRow);
        } else {
            ui->statusLabel->setText("Фотография выбрана: " + photoPath);
        }
    }
}

void MainWindow::on_findByName_triggered()
{
    QDialog searchDialog(this);
    searchDialog.setWindowTitle("Поиск книги");
    searchDialog.resize(400, 320);

    QVBoxLayout *layout = new QVBoxLayout(&searchDialog);

    QLabel *titleLabel = new QLabel("Введите данные для поиска:", &searchDialog);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");

    QLineEdit *nameEdit = new QLineEdit(&searchDialog);
    nameEdit->setPlaceholderText("Название книги");

    QLineEdit *genreEdit = new QLineEdit(&searchDialog);
    genreEdit->setPlaceholderText("Жанр");

    QLineEdit *authorEdit = new QLineEdit(&searchDialog);
    authorEdit->setPlaceholderText("Автор");

    QLineEdit *publisherEdit = new QLineEdit(&searchDialog);
    publisherEdit->setPlaceholderText("Издательство");

    QLabel *noteLabel = new QLabel("* Можно оставить поля пустыми для поиска по частичным совпадениям", &searchDialog);
    noteLabel->setStyleSheet("color: gray; font-size: 10px;");

    QPushButton *searchBtn = new QPushButton("Найти", &searchDialog);
    QPushButton *cancelBtn = new QPushButton("Отмена", &searchDialog);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(searchBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addWidget(titleLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(genreEdit);
    layout->addWidget(authorEdit);
    layout->addWidget(publisherEdit);
    layout->addWidget(noteLabel);
    layout->addLayout(btnLayout);

    connect(searchBtn, &QPushButton::clicked, [&]() {
        QString name = nameEdit->text().trimmed();
        QString genre = genreEdit->text().trimmed();
        QString author = authorEdit->text().trimmed();
        QString publisher = publisherEdit->text().trimmed();

        if (name.isEmpty() && genre.isEmpty() && author.isEmpty() && publisher.isEmpty()) {
            QMessageBox::information(&searchDialog, "Поиск", "Заполните хотя бы одно поле для поиска");
            return;
        }

        int foundRow = findBookByFields(name, genre, publisher, author);

        if (foundRow >= 0) {
            BookData book = getBookDataFromRow(foundRow);

            QDialog resultDialog(this);
            resultDialog.setWindowTitle("Книга найдена");
            resultDialog.resize(350, 250);

            QVBoxLayout *resultLayout = new QVBoxLayout(&resultDialog);

            QLabel *infoLabel = new QLabel(&resultDialog);
            infoLabel->setText(QString("Найдена книга:\n\n"
                                      "Название: %1\n"
                                      "Автор: %2\n"
                                      "Жанр: %3\n"
                                      "Издательство: %4\n"
                                      "Осталось в наличии: %5 шт.\n"
                                      "Цена: %6 руб.")
                                  .arg(book.name)
                                  .arg(book.author)
                                  .arg(book.genre)
                                  .arg(book.publisher)
                                  .arg(book.quantity)
                                  .arg(book.price, 0, 'f', 2));

            QPushButton *buyBtn = new QPushButton("Купить", &resultDialog);
            QPushButton *cancelResultBtn = new QPushButton("Отмена", &resultDialog);

            QHBoxLayout *resultBtnLayout = new QHBoxLayout();
            resultBtnLayout->addWidget(buyBtn);
            resultBtnLayout->addWidget(cancelResultBtn);

            resultLayout->addWidget(infoLabel);
            resultLayout->addLayout(resultBtnLayout);

            connect(buyBtn, &QPushButton::clicked, [&]() {
                updateTableAfterPurchase(foundRow);
                resultDialog.accept();
                searchDialog.accept();
            });

            connect(cancelResultBtn, &QPushButton::clicked, &resultDialog, &QDialog::reject);

            resultDialog.exec();
        } else {
            QMessageBox::information(&searchDialog, "Поиск", "Книга с указанными параметрами не найдена");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, &searchDialog, &QDialog::reject);

    searchDialog.exec();
}

void MainWindow::on_sortByAuthorAsc_triggered()
{
    ui->table->sortItems(2, Qt::AscendingOrder);
    ui->statusLabel->setText("Отсортировано по автору А...Я");
    loadPhotoForRow(ui->table->currentRow());
}

void MainWindow::on_sortByAuthorDesc_triggered()
{
    ui->table->sortItems(2, Qt::DescendingOrder);
    ui->statusLabel->setText("Отсортировано по автору Я...А");
    loadPhotoForRow(ui->table->currentRow());
}
