#ifndef SKLAD_H
#define SKLAD_H

#include <QWidget>
#include <QMouseEvent>
#include <QSortFilterProxyModel>

#include "menu.h"
#include "settyre.h"
#include "qjsontable.h"


namespace Ui {
class sklad;
}

class sklad : public QWidget
{
    Q_OBJECT

public:
    explicit sklad(QWidget *parent = 0);
    ~sklad();

private slots:
    void filterModel();

    void on_btnReturn_clicked();
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();

    void connectManagerSklad();
    void deleteRequest();

    void replyFinishedSklad(QNetworkReply* reply);
    void replyDeleteFinished(QNetworkReply* reply);

    void on_tableSklad_clicked(const QModelIndex &index);
    void on_btnRefresh_clicked();

    void imgIndicatorSklad();

    void mousePressEvent(QMouseEvent *event);

    void on_btnSearch_clicked();
    void on_searchQuerySklad_textChanged(const QString &arg1);
    void on_searchQuerySklad_editingFinished();

    void on_btnEdit_clicked();

private:
    Ui::sklad *ui;
    QNetworkAccessManager* manager;
    QJsonTableModel* tableSklad;
    QSortFilterProxyModel *proxyModel;
    bool isSkladRowSelected = false;
    QJsonObject objectIndexOfSkladSelected;
  //  QSortFilterProxyModel *proxyModel;

};

#endif // SKLAD_H
