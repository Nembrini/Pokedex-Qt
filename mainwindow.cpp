#include "mainwindow.h"
#include "pokemon.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QFile>
#include <QCheckBox>
#include <QStandardItem>
#include <QLineEdit>
#include <QtCharts/QChartView>



/*/-------------------------------------/*/
//                                      //
//  FILIPPO NEMBRINI SEBASTIANO 886135  //
//                                      //
/*/------------------------------------/*/



// Costruttore di MainWindow: Inizializza la finestra principale e imposta gli elementi dell'interfaccia utente.
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // Imposta il titolo della finestra
    this->setWindowTitle("Pokedex by Filippo Sebatiano Nembrini 886135");

    // Imposta le dimensioni della finestra
    this->resize(1024, 640);

    // Analizza il file CSV contenente i dati del Pokedex
    QList<QStringList> file_lettura = readFilePokedex();

    // Estrae le intestazioni della tabella dal file CSV analizzato
    QStringList intestazione_tabella = file_lettura[0];
    file_lettura.removeOne(intestazione_tabella);

    // Stampa ogni riga del file CSV analizzato per il debug
    foreach(QStringList row, file_lettura) {
        qDebug() << row;
    }

    // Popola _pokedex con gli oggetti Pokemon dal file CSV analizzato
    _pokedex = getListaPokemon(file_lettura);

    // Ottiene una lista di tutti i tipi di Pokemon
    _lista_tipi = getListaTipi();

    // Aggiunge i dati dei Pokemon alla tabella
    aggiungiPokemon(intestazione_tabella);

    // Stampa la lista dei tipi di Pokemon per il debug
    qDebug() << _lista_tipi;

    // Aggiunge i filtri di tipo all'interfaccia utente
    addFiltroTipo(_lista_tipi);

    // Popola le ComboBox dei Pokemon
    aggiungiPokemonSlotCominato();

    // Collega i segnali activated delle ComboBox alla funzione impostaDatiConfronto()
    connect(ui->pokemon1_combobox, SIGNAL(activated(int)), this, SLOT(impostaDatiConfronto()));
    connect(ui->pokemon2_combobox, SIGNAL(activated(int)), this, SLOT(impostaDatiConfronto()));

    // Collega il segnale currentChanged di tabWidget alla funzione cambioScheda()
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(cambioScheda(int)));

    // Genera un grafico vuoto
    _grafico = generaGraficoVuoto();
}



// Distruttore di MainWindow: Elimina l'interfaccia utente
MainWindow::~MainWindow()
{
    delete ui;
}



// La funzione tabChanged si attiva quando viene cambiata la tabella nell'interfaccia.
// Prende come argomento l'indice della nuova tabella selezionata.

void MainWindow::cambioScheda(int index)
{
    // Ottiene il puntatore alla barra di stato dall'interfaccia utente.
    QStatusBar *status_bar = ui->statusbar;

    // Se l'indice della tabella è 0, mostra la barra di stato, altrimenti la nasconde.
    if(index == 0) {
        status_bar->show();
    }
    else {
        status_bar->hide();
    }
}



// Questa funzione analizza il file CSV contenente i dati del Pokédex.
// Legge il file riga per riga, rimuove i caratteri di citazione e divide le righe in elementi separati da virgole.
// Restituisce una lista di liste di stringhe, rappresentanti le informazioni sui Pokémon.
QList<QStringList> MainWindow::readFilePokedex()
{
    QString percorso_pokedex = ":/csv/pokedex.csv";

    QFile file(percorso_pokedex);
    QStringList word_list;

    QTextStream stream(&file);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while(!stream.atEnd()) {
            QString line = stream.readLine();
            word_list.append(line);
        }
    }
    else {
        qDebug() << "File: " << file.exists();
        qDebug() << "Errore Stringa: " << file.errorString();
        qDebug() << "Errore: " << file.error();
    }

    QStringList riga_file;
    QList<QStringList> pokemon_info;

    for(QString& row: word_list) {
        riga_file = row.split('"');
        if(riga_file.size() > 1) {
            riga_file[1].replace(",", "|");
            riga_file[1] = riga_file[1].simplified().remove(" ");
        }
        row = riga_file.join("");
    }

    foreach(QString row, word_list) {
        riga_file = row.split(",");
        pokemon_info.append(riga_file);
    }

    return pokemon_info;
}



// Questa funzione restituisce una lista di tipi di Pokémon unici presenti nel Pokédex.
// Itera attraverso la lista dei Pokémon nel Pokédex e per ciascun Pokémon estrae i suoi tipi unici e li aggiunge alla lista dei tipi.
// Restituisce la lista dei tipi di Pokémon.
QStringList MainWindow::getListaTipi() {
    QStringList lista_tipi;

    foreach (Pokemon pokemon, _pokedex) {
        foreach(QString tipo, pokemon.getTypes())
            if(!lista_tipi.contains(tipo))
                lista_tipi.append(tipo);
    }

    return lista_tipi;
}



// Questa funzione aggiunge filtri di tipo alla GUI.
// Prende una lista di tipi come input e aggiunge un filtro di tipo per ciascun tipo nella lista alla layout dei filtri.
// Collega il segnale di clic del filtro alla slot di filtro.
void MainWindow::addFiltroTipo(QStringList types)
{
    QVBoxLayout *layout_filtri = ui->filtersLayout;
    foreach (QString tipo, types) {
        QCheckBox *filtro_tipo = new QCheckBox;
        filtro_tipo->setText(tipo);
        layout_filtri->addWidget(filtro_tipo);
        connect(filtro_tipo, SIGNAL(clicked(bool)), this, SLOT(filtra()));
    }
}



QString MainWindow::getColoreTipo(QStringList types, QString tipo)
{
    switch(types.indexOf(tipo)) {
    case 0: return "#77Be53"; // Tipo Grass
        break;
    case 1: return "#974f94"; // Tipo Poison
        break;
    case 2: return "#e77f34"; // Tipo Fire
        break;
    case 3: return "#a891e0"; // Tipo Flying
        break;
    case 4: return "#654fc7"; // Tipo Dragon
        break;
    case 5: return "#6890f1"; // Tipo Water
        break;
    case 6: return "#b8b920"; // Tipo Bug
        break;
    case 7: return "#a8b778"; // Tipo Normal
        break;
    case 8: return "#704748"; // Tipo Dark
        break;
    case 9: return "#e8d020"; // Tipo Electric
        break;
    case 10: return "#f85988"; // Tipo Psychic
        break;
    case 11: return "#f0d068"; // Tipo Ground
        break;
    case 12: return "#98f8e8"; // Tipo Ice
        break;
    case 13: return "#c8bbd0"; // Tipo Steel
        break;
    case 14: return "#e0b6bc"; // Tipo Fairy
        break;
    case 15: return "#c03038"; // Tipo Fighting
        break;
    case 16: return "#b8a138"; // Tipo Rock
        break;
    case 17: return "#705998"; // Tipo Ghost
        break;
    default: return "white; "  // Riquadro Default
               "border: 1px solid black; "
               "color: black;";
    }
}



// Funzione filtra(): Filtra i Pokemon in base ai tipi selezionati e aggiorna l'interfaccia utente di conseguenza.
void MainWindow::filtra()
{
    // Crea una lista per memorizzare i tipi selezionati
    QStringList tipi_selezionati;
    QVBoxLayout *layout_filtri = ui->filtersLayout;

    // Itera attraverso ciascuna checkbox del filtro e aggiunge i tipi selezionati alla lista
    for (int i = 0; i < layout_filtri->count(); ++i) {
        QCheckBox *checkbox = qobject_cast<QCheckBox *>(layout_filtri->itemAt(i)->widget());
        if (checkbox->isChecked())
            tipi_selezionati.append(checkbox->text());
    }

    // Stampa i tipi selezionati per il debug
    qDebug() << tipi_selezionati;

    // Lista per memorizzare gli oggetti Pokemon dopo il filtraggio per tipo
    QList<Pokemon> pokemon_filtrati_per_tipo;

    // Applica il filtraggio per tipo
    if (!(tipi_selezionati.isEmpty())) {
        // Filtra i Pokemon per qualsiasi tipo selezionato
            foreach (Pokemon pokemon, _pokedex) {
                foreach (QString tipo, tipi_selezionati) {
                    if (pokemon.getTypes().contains(tipo))
                        pokemon_filtrati_per_tipo.append(pokemon);
                }
            }

    } else {
        // Se nessun tipo è selezionato, includi tutti i Pokemon
        pokemon_filtrati_per_tipo = _pokedex;
    }

    // Lista per memorizzare i Pokemon filtrati finali
    QList<Pokemon> pokemon_filtrati;

    // Copia pokemon_filtrati_per_tipo in pokemon_filtrati
    foreach (Pokemon pokemon, pokemon_filtrati_per_tipo) {
        pokemon_filtrati.append(pokemon);
    }

    // Ottieni il widget della tabella
    QTableWidget *table = ui->pokemonTable;

    // Itera attraverso le righe della tabella e mostra/nascondi le righe in base al filtraggio
    for (int i = 0; i < table->rowCount(); ++i) {
        table->showRow(i);

        if (!(tipi_selezionati.isEmpty())) {
            table->hideRow(i);
            foreach (Pokemon pokemon, pokemon_filtrati) {
                if (table->item(i, 1)->text() == pokemon.getName()) {
                    table->showRow(i);
                }
            }
        }
    }

    // Mostra il numero di Pokemon visualizzati nella barra di stato
    ui->statusbar->showMessage(QStringLiteral("%1 pokemon mostrati.").arg(pokemon_filtrati.size()));
}



    // La funzione impostaDatiConfronto() si occupa di impostare i dati per il confronto tra due Pokémon.
    // Inizializza variabili e layout per i due Pokémon selezionati e aggiorna le informazioni e il grafico di confronto.

void MainWindow::impostaDatiConfronto()
{
    // Variabili per i combobox dei Pokémon selezionati
    QComboBox *combobox1 = ui->pokemon1_combobox;
    QComboBox *combobox2 = ui->pokemon2_combobox;

    // Etichette per le immagini dei Pokémon selezionati
    QLabel *pokemon1_img = ui->pokemon1_img;
    QLabel *pokemon2_img = ui->pokemon2_img;

    // Layout per i tipi dei Pokémon selezionati
    QHBoxLayout *layout_tipi1 = ui->pokemon1_types;
    QHBoxLayout *layout_tipi2 = ui->pokemon2_types;

    // Disabilita i Pokémon selezionati nei rispettivi combobox
    disbilitaPokemonSelezionato(combobox1, combobox2);
    disbilitaPokemonSelezionato(combobox2, combobox1);

    // Mostra le informazioni del Pokémon selezionato e i tipi nel layout
    mostraInfoPokemon(combobox1, pokemon1_img, layout_tipi1);
    mostraInfoPokemon(combobox2, pokemon2_img, layout_tipi2);

    // Rimuove tutte le serie dal grafico
    _grafico->removeAllSeries();

    // Aggiorna il grafico con le informazioni dei Pokémon selezionati
    if(combobox1->currentIndex() != -1) {
        Pokemon pokemon1 = _pokedex[combobox1->currentIndex()];
        aggiornaGrafico(_grafico, pokemon1, 60);
    }
    if(combobox2->currentIndex() != -1) {
        Pokemon pokemon2 = _pokedex[combobox2->currentIndex()];
        aggiornaGrafico(_grafico, pokemon2, 120);
    }
}



// La funzione mostraInfoPokemon() mostra le informazioni del Pokémon selezionato.
// Aggiorna l'immagine del Pokémon e mostra i tipi corrispondenti nel layout.

void MainWindow::mostraInfoPokemon(QComboBox* pokemon_cbox, QLabel* pokemon_img, QLayout* layout_tipi)
{
    // Pulisce il layout dei tipi
    pulisciLayout(layout_tipi);
    // Resetta lo stile e il testo dell'immagine del Pokémon
    pokemon_img->setStyleSheet("");
    pokemon_img->setText("Nessun Pokemon Selezionato");

    // Se è stato selezionato un Pokémon, aggiorna le informazioni
    if(pokemon_cbox->currentIndex() != -1) {
        Pokemon pokemon = _pokedex[pokemon_cbox->currentIndex()];
        // Imposta lo stile dell'immagine del Pokémon
        pokemon_img->setStyleSheet("image: url("
                                   ":img/" + pokemon.getUrlImg() +
                                   ");"
                                   "image-position: center center;"
                                   );
        // Cancella il testo dall'immagine del Pokémon
        pokemon_img->setText("");

        // Per ogni tipo del Pokémon, crea una nuova etichetta e aggiungila al layout dei tipi
        foreach (QString type, pokemon.getTypes()) {
            QLabel *label_tipo = new QLabel();
            label_tipo->setText(type);
            label_tipo->setAlignment(Qt::AlignCenter);
            // Imposta lo stile dell'etichetta del tipo
            label_tipo->setStyleSheet("QLabel { "
                                      "color: black;"
                                      "background-color : "
                                      + getColoreTipo(_lista_tipi, type) +
                                      ";"
                                      "padding: 5px;"
                                      "border-radius: 5px;"
                                      "font-size: 15pt;"
                                      "border: 1px solid black;"
                                      "text-transform: uppercase;"
                                      "}");

            label_tipo->setFixedSize(100, 50);

            layout_tipi->addWidget(label_tipo);
        }

    }
}



// La funzione disbilitaPokemonSelezionato() disabilita il Pokémon selezionato in un combobox in un altro combobox.
// Questo impedisce la selezione dello stesso Pokémon in entrambi i combobox.

void MainWindow::disbilitaPokemonSelezionato(QComboBox* pokemon1, QComboBox* pokemon2)
{
    // Ottiene il modello degli elementi del combobox
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(pokemon1->model());
    Q_ASSERT(model != nullptr);

    // Abilita tutti gli elementi nel combobox
    for(int i = 0; i < pokemon1->count(); i++) {
        QStandardItem *item = model->item(i);
        item->setEnabled(true);
    }

    // Se è stato selezionato un Pokémon nel secondo combobox, disabilita il corrispondente nel primo combobox
    if(pokemon2->currentIndex() != -1) {
        QStandardItem *item = model->item(pokemon2->currentIndex());
        item->setEnabled(false);
    }
}



// La funzione addPokemonToTable aggiunge i Pokémon alla tabella.
// Prende come argomento una lista di intestazioni per la tabella.
void MainWindow::aggiungiPokemon(QStringList headers)
{
    // Ottiene il puntatore alla tabella dei Pokémon dall'interfaccia utente.
    QTableWidget *table = ui->pokemonTable;

    // Imposta le caratteristiche della tabella.
    QFont bold_font;
    bold_font.setBold(true);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setFocusPolicy(Qt::NoFocus);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSortingEnabled(true);

    // Imposta il numero di righe e colonne della tabella.
    table->setRowCount(_pokedex.size());
    table->setColumnCount(headers.size()-1);

    // Rimuove il primo elemento dalle intestazioni.
    headers.removeAt(0);
    table->setHorizontalHeaderLabels(headers);

    // Nasconde le intestazioni verticali e imposta lo stile delle intestazioni orizzontali.
    table->verticalHeader()->hide();
    table->horizontalHeader()->setFont(bold_font);

    // Contatore delle righe della tabella.
    unsigned int conta_righe = 0;

    // Itera attraverso i Pokémon nel pokedex.
    foreach(Pokemon pokemon, _pokedex) {

        // Aggiunge il numero del Pokémon alla cella corrispondente.
        QTableWidgetItem *number = new QTableWidgetItem;
        number->setData(Qt::EditRole, pokemon.getNumber());
        table->setItem(conta_righe, 0 ,number);

        // Aggiunge il nome del Pokémon alla cella corrispondente.
        QTableWidgetItem *name = new QTableWidgetItem;
        name->setText(pokemon.getName());
        table->setItem(conta_righe, 1 ,name);

        // Aggiunge i tipi del Pokémon alla cella corrispondente.
        QTableWidgetItem* types = new QTableWidgetItem();
        table->setItem(conta_righe, 2, types);
        QWidget *colorTableWidget = new QWidget();

        QVBoxLayout *layout_tipi = new QVBoxLayout();

        // Itera attraverso i tipi del Pokémon e li aggiunge alla cella con lo stile appropriato.
        foreach (QString type, pokemon.getTypes()) {
            QLabel *label_tipo = new QLabel();
            label_tipo->setText(type);
            label_tipo->setAlignment(Qt::AlignCenter);

            label_tipo->setStyleSheet("QLabel { "
                                      "color: white;"
                                      "background-color : "
                                      + getColoreTipo(_lista_tipi, type) +
                                      ";"
                                      "padding: 5px;"
                                      "border-radius: 5px;"
                                      "font-weight:bold;"
                                      "text-transform: uppercase;"
                                      "}");

            layout_tipi->addWidget(label_tipo);
        }

        colorTableWidget->setLayout(layout_tipi);
        table->setCellWidget(conta_righe, 2 ,colorTableWidget);

        // Aggiunge le statistiche del Pokémon alle celle corrispondenti.
        QTableWidgetItem *total = new QTableWidgetItem;
        total->setData(Qt::EditRole, pokemon.getTotal());
        table->setItem(conta_righe, 3 ,total);

        QTableWidgetItem *hp = new QTableWidgetItem;
        hp->setData(Qt::EditRole, pokemon.getHp());
        table->setItem(conta_righe, 4 ,hp);

        QTableWidgetItem *atk = new QTableWidgetItem;
        atk->setData(Qt::EditRole, pokemon.getAtk());
        table->setItem(conta_righe, 5 ,atk);

        QTableWidgetItem *def = new QTableWidgetItem;
        def->setData(Qt::EditRole, pokemon.getDef());
        table->setItem(conta_righe, 6 ,def);

        QTableWidgetItem *spatk = new QTableWidgetItem;
        spatk->setData(Qt::EditRole, pokemon.getSpAtk());
        table->setItem(conta_righe, 7 ,spatk);

        QTableWidgetItem *spdef = new QTableWidgetItem;
        spdef->setData(Qt::EditRole, pokemon.getSpDef());
        table->setItem(conta_righe, 8 ,spdef);

        QTableWidgetItem *speed = new QTableWidgetItem;
        speed->setData(Qt::EditRole, pokemon.getSpeed());
        table->setItem(conta_righe, 9 ,speed);

        ++conta_righe;
    }

    // Imposta l'allineamento del testo e lo stile delle celle della tabella.
    for(int i = 0; i < table->rowCount(); ++i) {
        table->item(i, 0)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        table->item(i, 1)->setFont(bold_font);
        table->item(i, 1)->setForeground(QBrush(QColor("blue")));   //cambio il colore del nome del pokemon per far capire che è un collegamento
        table->item(i, 2)->setData(Qt::TextAlignmentRole, Qt::AlignCenter);
        table->item(i, 3)->setFont(bold_font);

        for(int j = 3; j < table->columnCount(); ++j) {
            table->item(i,j)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        }
    }

    // Ridimensiona le righe della tabella in base al contenuto e imposta la modalità di ridimensionamento delle colonne.
    table->resizeRowsToContents();
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Collega il segnale cellClicked della tabella al relativo slot.
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(clickTabella(int,int)));
}



// Questa funzione gestisce l'evento di clic sulla tabella dei Pokémon.
// Quando un elemento della tabella viene cliccato, questa funzione viene chiamata con le coordinate della cella.
// Estrae il testo dalla cella selezionata e verifica se il testo corrisponde a un elemento nella combobox dei Pokémon.
// Se il testo è presente, imposta l'indice della tabella delle schede alla seconda scheda,
// imposta l'indice della combobox dei Pokémon alla posizione del Pokémon selezionato,
// imposta l'indice della combobox dei secondi Pokémon a -1 e chiama la funzione impostaDatiConfronto() per aggiornare i dati della comparazione.
void MainWindow::clickTabella(int row, int column)
{
    QString testo_cella = ui->pokemonTable->item(row, column)->text();

    QComboBox* combobox = ui->pokemon1_combobox;
    if(combobox->findText(testo_cella) != -1) {
        ui->tabWidget->setCurrentIndex(1);
        combobox->setCurrentIndex(combobox->findText(testo_cella));
        ui->pokemon2_combobox->setCurrentIndex(-1);
        impostaDatiConfronto();
    }
}



// Questa funzione crea una lista di oggetti Pokemon utilizzando le informazioni fornite in parsed_file.
// Itera attraverso la lista delle informazioni sui Pokémon e chiama la funzione creaPokemon() per creare un oggetto Pokemon per ciascuna riga.
// Restituisce la lista degli oggetti Pokemon creati.
QList<Pokemon> MainWindow::getListaPokemon(QList<QStringList> parsed_file)
{
    QList<Pokemon> lista_pokemon;

    foreach(QStringList statistiche_pokemon, parsed_file) {
        lista_pokemon.append(creaPokemon(statistiche_pokemon));
    }

    foreach(Pokemon pokemon, lista_pokemon) {
        qDebug() << pokemon;
    }

    return lista_pokemon;
}



// Questa funzione crea un oggetto Pokemon utilizzando le informazioni fornite in statistiche_pokemon.
// Estrae i singoli attributi dall'elenco delle statistiche e crea un nuovo oggetto Pokemon con essi.
// Restituisce l'oggetto Pokemon appena creato.
Pokemon MainWindow::creaPokemon(QStringList statistiche_pokemon)
{
    QString url_img = statistiche_pokemon[0];
    unsigned int number = statistiche_pokemon[1].toInt();
    QString name = statistiche_pokemon[2];
    QStringList types = statistiche_pokemon[3].split("|");
    unsigned int total = statistiche_pokemon[4].toInt();
    unsigned int hp = statistiche_pokemon[5].toInt();
    unsigned int atk = statistiche_pokemon[6].toInt();
    unsigned int def = statistiche_pokemon[7].toInt();
    unsigned int sp_atk = statistiche_pokemon[8].toInt();
    unsigned int sp_def = statistiche_pokemon[9].toInt();
    unsigned int speed = statistiche_pokemon[10].toInt();

    Pokemon tmp(url_img, number, name, types, total, hp, atk, def, sp_atk, sp_def, speed);
    return tmp;
}



// Questa funzione aggiunge i Pokémon presenti nella lista `_pokedex` ai due QComboBox presenti nell'interfaccia utente.
void MainWindow::aggiungiPokemonSlotCominato()
{
    // Otteniamo i puntatori ai QComboBox dalla UI.
    QComboBox *pokemon1 = ui->pokemon1_combobox;
    QComboBox *pokemon2 = ui->pokemon2_combobox;

    // Iteriamo attraverso la lista dei Pokémon e aggiungiamo i loro nomi ai QComboBox.
    foreach (Pokemon pokemon, _pokedex) {
        pokemon1->addItem(pokemon.getName());
        pokemon2->addItem(pokemon.getName());
    }

    // Impostiamo l'indice corrente dei QComboBox su -1, abilitiamo la modifica e impostiamo un testo placeholder.
    pokemon1->setCurrentIndex(-1);
    pokemon1->setEditable(true);
    pokemon1->setInsertPolicy(QComboBox::NoInsert);
    pokemon1->lineEdit()->setPlaceholderText(QStringLiteral("Seleziona un Pokémon:"));
    pokemon1->setStyleSheet("border-radius:14px;"
                            "border:2px solid #D3D3D3;"
                            "padding:4px;"
                            "font-weight:bold;"
                            "color:black;");

    pokemon2->setCurrentIndex(-1);
    pokemon2->setEditable(true);
    pokemon2->setInsertPolicy(QComboBox::NoInsert);
    pokemon2->lineEdit()->setPlaceholderText(QStringLiteral("Seleziona un Pokémon:"));
    pokemon2->setStyleSheet("border-radius:14px;"
                            "border:2px solid #D3D3D3;"
                            "padding:4px;"
                            "font-weight:bold;"
                            "color:black");
}



// Questa funzione svuota un layout rimuovendo tutti i widget al suo interno.
void MainWindow::pulisciLayout(QLayout* layout)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (QWidget* widget = item->widget()) {
            delete widget;
        }
    }
}



// Questa funzione calcola e restituisce il valore percentuale di un valore rispetto al massimo valore dato.
double MainWindow::getPercentuale(unsigned int value, unsigned int max_value)
{
    return static_cast<double>(value)/max_value*100;
}



// Questa funzione genera e restituisce un nuovo grafico polare vuoto.
QPolarChart* MainWindow::generaGraficoVuoto()
{
    // Creiamo un nuovo QPolarChart e lo aggiungiamo a un QChartView.
    QPolarChart *chart = new QPolarChart();
    QChartView *chartView = new QChartView();
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Creiamo e configuriamo gli assi angolare e radiale del grafico.
    QCategoryAxis *angularAxis = new QCategoryAxis;
    angularAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    angularAxis->setRange(0, 360);
    angularAxis->append("Totale", 0);
    angularAxis->append("Velocità", 1*(360/7));
    angularAxis->append("Dif. Speciale", 2*(360/7));
    angularAxis->append("Att. Speciale", 3*(360/7));
    angularAxis->append("Difesa", 4*(360/7));
    angularAxis->append("Attacco", 5*(360/7));
    angularAxis->append("Punti Salute", 6*(360/7));

    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setTickCount(11);
    radialAxis->setLabelFormat("%d");
    radialAxis->setRange(0, 100);

    // Aggiungiamo gli assi al grafico.
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);
    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);

    // Aggiungiamo il QChartView al layout definito nella UI e restituiamo il grafico.
    ui->chart_layout->addWidget(chartView);

    return chart;
}



// Questa funzione aggiorna la serie del grafico polare con i dati del Pokemon dato.
void MainWindow::aggiornaGrafico(QPolarChart* chart, Pokemon pokemon, unsigned int brightness)
{
    // Inizializzazione delle variabili per i valori massimi di ogni statistica.
    unsigned int max_total = 0;
    unsigned int max_hp = 0;
    unsigned int max_atk = 0;
    unsigned int max_def = 0;
    unsigned int max_sp_atk = 0;
    unsigned int max_sp_def = 0;
    unsigned int max_speed = 0;

    // Calcolo dei valori massimi per ogni statistica attraverso i Pokemon nel Pokedex.
    foreach (Pokemon pokemon, _pokedex) {
        if(pokemon.getTotal()>max_total) {max_total = pokemon.getTotal();}
        if(pokemon.getHp()>max_hp) {max_hp = pokemon.getHp();}
        if(pokemon.getAtk()>max_atk) {max_atk = pokemon.getAtk();}
        if(pokemon.getDef()>max_def) {max_def = pokemon.getDef();}
        if(pokemon.getSpAtk()>max_sp_atk) {max_sp_atk = pokemon.getSpAtk();}
        if(pokemon.getSpDef()>max_sp_def) {max_sp_def = pokemon.getSpDef();}
        if(pokemon.getSpeed()>max_speed) {max_speed = pokemon.getSpeed();}
    }

    // Creazione di un oggetto Pokemon con i valori massimi.
    Pokemon max("", 0, "", {}, max_total, max_hp, max_atk, max_def, max_sp_atk, max_sp_def, max_speed);

    // Generazione della serie di dati per il grafico.
    QLineSeries *series = generaDatiSerie(pokemon, max);
    // Impostazione del colore della serie.
    impostColoreSerie(series, pokemon, brightness);

    // Aggiunta della serie al grafico.
    chart->addSeries(series);

    // Attacco della serie agli assi del grafico.
    const QList<QAbstractAxis *> axisList = chart->axes();

    for (QAbstractAxis *axis : axisList) {
        series->attachAxis(axis);
    }
}



// Questa funzione imposta il colore della serie della linea.
void MainWindow::impostColoreSerie(QLineSeries* series, Pokemon pokemon, int brightness)
{
    // Impostazione dello stile della penna per la serie.
    QPen pen = series->pen();
    pen.setWidth(2);
    series->setPen(pen);
    // Ottenimento del colore principale del tipo del Pokemon.
    QString color_hex(getColoreTipo(_lista_tipi, pokemon.getTypes().at(0)));
    // Se il colore è bianco, lo sostituisci con nero.
    if(color_hex == "white; border: 1px solid grey; color: black;")
        color_hex = "#000";
    QColor *color = new QColor(color_hex);
    // Applicazione del colore più chiaro alla serie.
    series->setColor(color->lighter(brightness).name());
}



// Questa funzione genera la serie di dati per il grafico polare.
QLineSeries *MainWindow::generaDatiSerie(Pokemon pokemon, Pokemon max)
{
    // Creazione di una nuova serie.
    QLineSeries *series = new QLineSeries();
    // Aggiunta dei punti per ogni statistica relativa al Pokemon.
    series->append(0, getPercentuale(pokemon.getTotal(), max.getTotal()));
    series->append(1*(360/7), getPercentuale(pokemon.getSpeed(), max.getSpeed()));
    series->append(2*(360/7), getPercentuale(pokemon.getSpDef(), max.getSpDef()));
    series->append(3*(360/7), getPercentuale(pokemon.getSpAtk(), max.getSpAtk()));
    series->append(4*(360/7), getPercentuale(pokemon.getDef(), max.getDef()));
    series->append(5*(360/7), getPercentuale(pokemon.getAtk(), max.getAtk()));
    series->append(6*(360/7), getPercentuale(pokemon.getHp(), max.getHp()));
    series->append(360, getPercentuale(pokemon.getTotal(), max.getTotal()));
    series->setName(pokemon.getName());

    return series;
}
