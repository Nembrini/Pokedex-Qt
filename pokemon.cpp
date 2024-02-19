#include "pokemon.h"



/*/-------------------------------------/*/
//                                      //
//  FILIPPO NEMBRINI SEBASTIANO 886135  //
//                                      //
/*/------------------------------------/*/



// Costruttore predefinito
Pokemon::Pokemon() :
    _url_img(""),
    _number(0),
    _name(""),
    _types({""}),
    _total(0),
    _hp(0),
    _atk(0),
    _def(0),
    _sp_atk(0),
    _sp_def(0),
    _speed(0)
{}



// Costruttore con argomenti
Pokemon::Pokemon(
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
    int speed)
    :
    _url_img(url_img),
    _number(number),
    _name(name),
    _types(types),
    _total(total),
    _hp(hp),
    _atk(atk),
    _def(def),
    _sp_atk(sp_atk),
    _sp_def(sp_def),
    _speed(speed)
{}



// Operatore di conversione a stringa
Pokemon::operator QString() const
{
    QString pokemon_string = _url_img + ", "
                             + QString::number(_number) + ", "
                             + _name + ", "
                             + "";

    pokemon_string += "(";
    foreach (QString type, _types) {
        pokemon_string += type;
        if(_types.indexOf(type) != _types.size()-1)
            pokemon_string += ",";
    }
    pokemon_string += "), ";

    pokemon_string += QString::number(_total) + ", "
                      + QString::number(_hp) + ", "
                      + QString::number(_atk) + ", "
                      + QString::number(_def) + ", "
                      + QString::number(_sp_atk) + ", "
                      + QString::number(_sp_def) + ", "
                      + QString::number(_speed);

    return pokemon_string;
}



// Operatore di confronto di uguaglianza
bool Pokemon::operator==(const Pokemon &pokemon) const
{
    return _url_img == pokemon._url_img &&
           _number == pokemon._number &&
           _name == pokemon._name &&
           _types == pokemon._types &&
           _total == pokemon._total &&
           _hp == pokemon._hp &&
           _atk == pokemon._atk &&
           _def == pokemon._def &&
           _sp_atk == pokemon._sp_atk &&
           _sp_def == pokemon._sp_def &&
           _speed == pokemon._speed;
}



// Metodo getter per l'URL dell'immagine
QString Pokemon::getUrlImg()  {return _url_img;}

// Metodo getter per il numero del Pokemon
unsigned int Pokemon::getNumber()  {return _number;}

// Metodo getter per il nome del Pokemon
QString Pokemon::getName()  {return _name;}

// Metodo getter per i tipi del Pokemon
QStringList Pokemon::getTypes()  {return _types;}

// Metodo getter per il totale delle statistiche del Pokemon
unsigned int Pokemon::getTotal()  {return _total;}

// Metodo getter per i punti salute (HP) del Pokemon
unsigned int Pokemon::getHp()  {return _hp;}

// Metodo getter per l'attacco (Attack) del Pokemon
unsigned int Pokemon::getAtk()  {return _atk;}

// Metodo getter per la difesa (Defense) del Pokemon
unsigned int Pokemon::getDef()  {return _def;}

// Metodo getter per l'attacco speciale (Special Attack) del Pokemon
unsigned int Pokemon::getSpAtk()  {return _sp_atk;}

// Metodo getter per la difesa speciale (Special Defense) del Pokemon
unsigned int Pokemon::getSpDef()  {return _sp_def;}

// Metodo getter per la velocità (Speed) del Pokemon
unsigned int Pokemon::getSpeed()  {return _speed;}
