#include <SFML/Graphics.hpp>
#include <iostream>

int wh = 600; //laua laius/kõrgus

int w_ruut = wh/9; //1 ruudu laius/kõrgus


/**
 * @brief tagastab ruudu indexid, mille sisse jäävad (akna) Koordinaadid x ja y 
 * 
 * @param x 
 * @param y 
 * @return std::pair<int,int> 
 */
std::pair<int,int> getRuutMilleSeesKoordinaadid(float x, float y){
    for (int i = 0; i < 9; i++)
    {
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
    return std::make_pair(-1,-1);
}

int main()
{
    int laud[9][9] = //0 - pole määratud nrit
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

    std::pair<int,int> selekteeritud = std::make_pair(-1,-1); //näitab milline ruut on selekteeritud (x ja y indeksid)

    //std::cout << ":(";

    //INIT JOONISTATAV

    sf::RenderWindow window(sf::VideoMode(600, 600), "sudoku");

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
    
    sf::RectangleShape selekteeritud_highlight(sf::Vector2f(w_ruut-0, w_ruut-1)); //orans taust selekteeritud ruudule
    selekteeritud_highlight.setFillColor({230, 153, 0});


    sf::Font font;
    font.loadFromFile("SuPostcode-VGeLe.ttf");
    
    sf::Text nrid[9][9];//joonistamiseks Text elemendid mis näitavad numbreid laual
    
    //väärtustan etteantud laua järgi mittemuudetavad nrid
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            nrid[i][j] = sf::Text("", font);

            if(laud[i][j] == 0){
                muudetav[i][j] = true;
            }else{ //ette antud numbrid mida muuta ei saa
                nrid[i][j].setString(std::to_string(laud[i][j]));
                nrid[i][j].setStyle(sf::Text::Bold);
                nrid[i][j].setFillColor({56,64,45});
            }
            
            //
            nrid[i][j].setPosition((j*w_ruut)+11,(i*w_ruut)+2);
            nrid[i][j].setCharacterSize(50);
            
        }
    }

    //TAUST prerender
    sf::RenderTexture taust;
    if (!taust.create(wh, wh))
    {
        // error...
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

    //TODO muutumatud nr'id

    taust.display();
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
            
            if(event.type == sf::Event::MouseButtonReleased){
                if (event.mouseButton.button == sf::Mouse::Left){
                    std::pair ruut = getRuutMilleSeesKoordinaadid(event.mouseButton.x,event.mouseButton.y);
                    if(muudetav[ruut.first][ruut.second]){
                        selekteeritud.first = ruut.first;
                        selekteeritud.second = ruut.second;
                        selekteeritud_highlight.setPosition(sf::Vector2f((ruut.second*w_ruut)+0,(ruut.first*w_ruut)+1));
                        
                        //laud[6][6]++;
                        //nrid[6][6].setString(std::to_string(laud[6][6]));
                    }
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
                if(laud[i][j] != 0){
                    window.draw(nrid[i][j]);
                }
            }
        }
        
        window.display();
    }

    return 0;
}

