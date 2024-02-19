#ifndef POKEMON_H
#define POKEMON_H

#include <QString>
#include <QStringList>



/*/-------------------------------------/*/
//                                      //
//  FILIPPO NEMBRINI SEBASTIANO 886135  //
//                                      //
/*/------------------------------------/*/



using namespace std;

class Pokemon
{
    // URL dell'immagine del Pokemon
    QString _url_img;

    // Numero identificativo del Pokemon
    unsigned int _number;

    // Nome del Pokemon
    QString _name;

    // Lista dei tipi del Pokemon
    QStringList _types;

    // Valore totale delle statistiche del Pokemon
    unsigned int _total;

    // Punti salute del Pokemon
    unsigned int _hp;

    // Valore dell'attacco del Pokemon
    unsigned int _atk;

    // Valore della difesa del Pokemon
    unsigned int _def;

    // Valore dell'attacco speciale del Pokemon
    unsigned int _sp_atk;

    // Valore della difesa speciale del Pokemon
    unsigned int _sp_def;

    // Velocità del Pokemon
    unsigned int _speed;



public:
    // Costruttore predefinito della classe Pokemon
    Pokemon();

    // Costruttore della classe Pokemon che accetta argomenti per inizializzare gli attributi
    Pokemon(
        QString url_img,
        int number,
        QString name,
        QStringList types,
        int total,
        int hp,
        int atk,
        int def,
        int sp_atk,
        int sp_def,
        int speed);



    // Operatore di conversione a QString per convertire l'oggetto Pokemon in una stringa
    operator QString() const;

    // Operatore di confronto per verificare l'uguaglianza tra due oggetti Pokemon
    bool operator==(const Pokemon &pokemon) const;

    // Restituisce l'URL dell'immagine del Pokemon
    QString getUrlImg();

    // Restituisce il numero identificativo del Pokemon
    unsigned int getNumber();

    // Restituisce il nome del Pokemon
    QString getName();

    // Restituisce la lista dei tipi del Pokemon
    QStringList getTypes();

    // Restituisce il valore totale delle statistiche del Pokemon
    unsigned int getTotal();

    // Restituisce i punti salute del Pokemon
    unsigned int getHp();

    // Restituisce il valore dell'attacco del Pokemon
    unsigned int getAtk();

    // Restituisce il valore della difesa del Pokemon
    unsigned int getDef();

    // Restituisce il valore dell'attacco speciale del Pokemon
    unsigned int getSpAtk();

    // Restituisce il valore della difesa speciale del Pokemon
    unsigned int getSpDef();

    // Restituisce la velocità del Pokemon
    unsigned int getSpeed();
};

#endif // POKEMON_H
