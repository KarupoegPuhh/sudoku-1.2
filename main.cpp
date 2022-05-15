#include <SFML/Graphics.hpp>
#include <iostream>

int wh = 600; //laua laius/kõrgus

int w_ruut = wh/9; //1 ruudu laius/kõrgus

int laud[9][9] //kõik numbrid sudoku laual, 0 - pole määratud nrit
 = //hetkel algväärtustan mängu siin, TODO võimalus väärtustada/importida .ss vms failist sudoku algseis
{
    {5, 3, 4, 0, 7, 8, 0, 0, 2},
    {0, 7, 0, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {0, 5, 9, 7, 0, 0, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {0, 6, 0, 5, 3, 7, 0, 8, 4},
    {2, 8, 0, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

bool muudetav[9][9]; //kas sel kohal on number muudetav

std::pair<int,int> selekteeritud = std::make_pair(-1,-1); //näitab milline ruut on selekteeritud (rea ja veeru indeksid)

//JOONISTAMISEKS
sf::RenderWindow window(sf::VideoMode(wh, wh), "sudoku");
sf::RenderTexture taust;
sf::Text nrid[9][9];//Text elemendid mis näitavad numbreid laual

/**
 * @brief seab valmis enamuse ekraanile joonistatava (taust, numbrid)
 * EI OLE KASUTUSES
 */
void initJoonistatav(){ //sf::Texture
    
    sf::Vertex trellid[20][2]; //jooned numbrite vahel
    for (int i = 0; i < 10; i++)
    {
        //horisontaalsed
        trellid[i][0] = sf::Vertex(sf::Vector2f(0, w_ruut*i));
        trellid[i][1] = sf::Vertex(sf::Vector2f(wh, w_ruut*i));
        //vertikaalsed
        trellid[i+10][0] = sf::Vertex(sf::Vector2f(w_ruut*i, 0));
        trellid[i+10][1] = sf::Vertex(sf::Vector2f(w_ruut*i, wh));
    }

    sf::Font font; //nr'ite font
    font.loadFromFile("SuPostcode-VGeLe.ttf");
    
    //väärtustan etteantud laua järgi mittemuudetavad nrid
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            nrid[i][j] = sf::Text("", font);

            if(laud[i][j] == 0){
                muudetav[i][j] = true;
            }else{ //ette antud numbrid mida muuta ei saa
                muudetav[i][j] = false;
                nrid[i][j].setString(std::to_string(laud[i][j]));
                nrid[i][j].setStyle(sf::Text::Bold);
                nrid[i][j].setFillColor({56,84,45});
            }
            
            //et nr'id oleksid kenasti ruutude sees
            nrid[i][j].setPosition((j*w_ruut)+11,(i*w_ruut)+2);
            nrid[i][j].setCharacterSize(50);
            
        }
    }

    //TAUST prerender
    
    if (!taust.create(wh, wh))
    {
        std::cout << "taustaga error" << std::endl;
        //return;
    }

    taust.clear();
    
    //3x3 eristav taustavärv
    sf::RectangleShape kolmx_rectangle(sf::Vector2f(3*w_ruut, 3*w_ruut));
    kolmx_rectangle.setFillColor({22,22,22});

    kolmx_rectangle.setPosition(sf::Vector2f(0*w_ruut,3*w_ruut));
    taust.draw(kolmx_rectangle);

    kolmx_rectangle.setPosition(sf::Vector2f(3*w_ruut,0*w_ruut));
    taust.draw(kolmx_rectangle);

    kolmx_rectangle.setPosition(sf::Vector2f(3*w_ruut,6*w_ruut));
    taust.draw(kolmx_rectangle);

    kolmx_rectangle.setPosition(sf::Vector2f(6*w_ruut,3*w_ruut));
    taust.draw(kolmx_rectangle);

    //jooned
    for (int i = 0; i < 20; i++){
        taust.draw(trellid[i], 2, sf::Lines);
    }

    //TODO muutumatud nr'id, et ei peaks iga kord eraldi neid joonistama
    
    taust.display();
    //return taust.getTexture();
}

/**
 * @brief tagastab ruudu indexid (rida ja veerg), mille sisse jäävad (akna) Koordinaadid x ja y 
 * 
 * @param x 
 * @param y 
 * @return std::pair<int,int>
 */
std::pair<int,int> getRuutMilleSeesKoordinaadid(float x, float y){
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if(
                x < (i+1)*w_ruut && 
                x > i*w_ruut &&

                y < (j+1)*w_ruut && 
                y > j*w_ruut
            ){
                return std::make_pair(j,i);
            }
        }
    }
    return std::make_pair(-1,-1);//ei leidnud
}

/**
 * @brief muudab numbri ruudus mis on kohal pos uueks (uus_nr) ja valideerib sudoku laua
 * 
 * @param pos täisarvude paar (rida ja veerg)
 * @param uus_nr 
 */
void muudaNr(std::pair<int, int> pos, int uus_nr){
    laud[pos.first][pos.second] = uus_nr;
    //TODO valideeri
    nrid[pos.first][pos.second].setString(std::to_string(uus_nr));
}

int main()
{
    //std::cout << "kui liigutan initJoonistatava enda funktsiooni siis programm crashib või joonistab ekraanile valesti ja terminal ka millegipärast ei tööta kuigi üritasin teha debug build, seega vabandan et pole kood funktsioonis" << std::endl;
    
    //INIT JOONISTATAV
    sf::RectangleShape selekteeritud_highlight(sf::Vector2f(w_ruut-1, w_ruut-1)); //orans taust selekteeritud ruudule
    selekteeritud_highlight.setFillColor({230, 153, 0});

    sf::Font font; //nr'ite font
    font.loadFromFile("SuPostcode-VGeLe.ttf");

    bool initFunktsioonEiToota = true;
    if(initFunktsioonEiToota){//initJoonistatav() sisu on siin;
        sf::Vertex trellid[20][2]; //jooned numbrite vahel
        for (int i = 0; i < 10; i++)
        {
            //horisontaalsed
            trellid[i][0] = sf::Vertex(sf::Vector2f(0, w_ruut*i));
            trellid[i][1] = sf::Vertex(sf::Vector2f(wh, w_ruut*i));
            //vertikaalsed
            trellid[i+10][0] = sf::Vertex(sf::Vector2f(w_ruut*i, 0));
            trellid[i+10][1] = sf::Vertex(sf::Vector2f(w_ruut*i, wh));
        }

        
        
        //väärtustan etteantud laua järgi mittemuudetavad nrid
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                nrid[i][j] = sf::Text("", font);

                if(laud[i][j] == 0){
                    muudetav[i][j] = true;
                    nrid[i][j].setFillColor({56,84,45}); //muudetava värv
                }else{ //ette antud numbrid mida muuta ei saa
                    muudetav[i][j] = false;
                    nrid[i][j].setString(std::to_string(laud[i][j]));
                    nrid[i][j].setStyle(sf::Text::Bold);
                    nrid[i][j].setFillColor({200,204,200}); //const värv
                }
                
                //et nr'id oleksid kenasti ruutude sees
                nrid[i][j].setPosition((j*w_ruut)+11,(i*w_ruut)+2);
                nrid[i][j].setCharacterSize(50);
                
            }
        }

        //TAUST prerender
        
        if (!taust.create(wh, wh))
        {
            std::cout << "taustaga error" << std::endl;
            //return;
        }

        taust.clear();
        
        //3x3 eristav taustavärv
        sf::RectangleShape kolmx_rectangle(sf::Vector2f(3*w_ruut, 3*w_ruut));
        kolmx_rectangle.setFillColor({49,22,22});

        kolmx_rectangle.setPosition(sf::Vector2f(0*w_ruut,3*w_ruut));
        taust.draw(kolmx_rectangle);

        kolmx_rectangle.setPosition(sf::Vector2f(3*w_ruut,0*w_ruut));
        taust.draw(kolmx_rectangle);

        kolmx_rectangle.setPosition(sf::Vector2f(3*w_ruut,6*w_ruut));
        taust.draw(kolmx_rectangle);

        kolmx_rectangle.setPosition(sf::Vector2f(6*w_ruut,3*w_ruut));
        taust.draw(kolmx_rectangle);

        //jooned
        for (int i = 0; i < 20; i++){
            taust.draw(trellid[i], 2, sf::Lines);
        }

        //TODO muutumatud nr'id, et ei peaks iga kord eraldi neid joonistama
        
        taust.display();
    }else{
        //initJoonistatav();
    }
    
    const sf::Texture& taust_texture = taust.getTexture();
    sf::Sprite taust_sprite(taust_texture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            //akna sulgemise event
            if(event.type == sf::Event::Closed)
                window.close();
            
            //hiire vasak klikk event
            if(event.type == sf::Event::MouseButtonReleased){
                if (event.mouseButton.button == sf::Mouse::Left){
                    std::pair ruut = getRuutMilleSeesKoordinaadid(event.mouseButton.x,event.mouseButton.y);
                    if(muudetav[ruut.first][ruut.second]){ //klikitud ruut selekteeritakse kui see on muudetav
                        selekteeritud.first = ruut.first;
                        selekteeritud.second = ruut.second;

                        selekteeritud_highlight.setPosition(sf::Vector2f((ruut.second*w_ruut)+0,(ruut.first*w_ruut)+1));
                    }
                }
            }
            
            //numbriklahvide event
            if(event.type == sf::Event::KeyReleased && selekteeritud.first != -1 && selekteeritud.second != -1){ //on midagi selekteeritud
                //ei viitsinud elegantsemalt ega isegi switch teha
                if (event.key.code == sf::Keyboard::Num1){
                    muudaNr(selekteeritud, 1);
                }else if (event.key.code == sf::Keyboard::Num2){
                    muudaNr(selekteeritud, 2);
                }else if (event.key.code == sf::Keyboard::Num3){
                    muudaNr(selekteeritud, 3);
                }else if (event.key.code == sf::Keyboard::Num4){
                    muudaNr(selekteeritud, 4);
                }else if (event.key.code == sf::Keyboard::Num5){
                    muudaNr(selekteeritud, 5);
                }else if (event.key.code == sf::Keyboard::Num6){
                    muudaNr(selekteeritud, 6);
                }else if (event.key.code == sf::Keyboard::Num7){
                    muudaNr(selekteeritud, 7);
                }else if (event.key.code == sf::Keyboard::Num8){
                    muudaNr(selekteeritud, 8);
                }else if (event.key.code == sf::Keyboard::Num9){
                    muudaNr(selekteeritud, 9);
                }else if (event.key.code == sf::Keyboard::Num0){
                    muudaNr(selekteeritud, 0);
                }
            }
        }

        //JOONISTAMINE

        window.clear();
        //laua taust
        window.draw(taust_sprite);

        //selekteeritud ruut
        if(std::get<0>(selekteeritud) >= 0 && std::get<1>(selekteeritud) >= 0)
            window.draw(selekteeritud_highlight);

        //numbrid
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if(laud[i][j] != 0){//kui muutumatud tausta sees siis && muudetav[i][j]
                    window.draw(nrid[i][j]);
                }
            }
        }
        
        window.display();
    }

    return 0;
}

