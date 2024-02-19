#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Inclusione del file header della classe Pokemon
#include "pokemon.h"

#include <QMainWindow>
#include <QtCharts/QPolarChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QComboBox>
#include <QLabel>
#include <QString>
#include <QStringList>



/*/-------------------------------------/*/
//                                      //
//  FILIPPO NEMBRINI SEBASTIANO 886135  //
//                                      //
/*/------------------------------------/*/



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT



private:
    // Puntatore all'interfaccia utente
    Ui::MainWindow *ui;

    // Lista di oggetti Pokemon
    QList<Pokemon> _pokedex;

    // Lista dei tipi di Pokemon
    QStringList _lista_tipi;

    // Puntatore a un grafico polare
    QPolarChart* _grafico;

    // Metodo per analizzare un file CSV contenente i dati del Pokedex
    QList<QStringList> readFilePokedex();

    // Metodo per ottenere una lista di oggetti Pokemon dai dati analizzati
    QList<Pokemon> getListaPokemon(QList<QStringList> parsed_file);

    // Metodo per creare un oggetto Pokemon da una lista di statistiche
    Pokemon creaPokemon(QStringList pokemon_stats);

    // Metodo per ottenere una lista di tutti i tipi di Pokemon presenti nel Pokedex
    QStringList getListaTipi();

    // Metodo per aggiungere un Pokemon alla tabella
    void aggiungiPokemon(QStringList headers);

    // Metodo per ottenere il colore associato a un tipo di Pokemon
    QString getColoreTipo(QStringList types, QString type);

    // Metodo per aggiungere filtri per i tipi di Pokemon
    void addFiltroTipo(QStringList types);

    // Metodo per aggiungere i Pokemon a una casella combinata
    void aggiungiPokemonSlotCominato();

    // Metodo per visualizzare le informazioni di un Pokemon selezionato
    void mostraInfoPokemon(QComboBox* pokemon_cbox, QLabel* pokemon_img, QLayout* types_layout);

    // Metodo per disabilitare un Pokemon selezionato in altre caselle combinate
    void disbilitaPokemonSelezionato(QComboBox* pokemon1, QComboBox* pokemon2);

    // Metodo per eliminare tutti gli elementi da un layout
    void pulisciLayout(QLayout* layout);

    // Metodo per generare un grafico polare vuoto
    QPolarChart* generaGraficoVuoto();

    // Metodo per aggiornare le serie del grafico con i dati di un Pokemon
    void aggiornaGrafico(QPolarChart* chart, Pokemon pokemon, unsigned int brightness);

    // Metodo per impostare il colore di una serie del grafico
    void impostColoreSerie(QLineSeries* series, Pokemon pokemon, int brightness);

    // Metodo per generare una serie di dati per un Pokemon
    QLineSeries* generaDatiSerie(Pokemon pokemon, Pokemon max);

    // Metodo per ottenere il valore percentuale di un dato rispetto al massimo
    double getPercentuale(unsigned int value, unsigned int max_value);



public:
    // Costruttore della classe
    MainWindow(QWidget *parent = nullptr);

    // Distruttore della classe
    ~MainWindow();



public slots:
    // Slot per filtrare i dati
    void filtra();

    // Slot per impostare i dati di confronto
    void impostaDatiConfronto();

    // Slot chiamato quando viene cambiata la scheda
    void cambioScheda(int index);

    // Slot chiamato quando viene cliccata una cella della tabella
    void clickTabella(int riga, int column);
};

#endif // MAINWINDOW_H
