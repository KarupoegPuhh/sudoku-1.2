#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{


    int wh = 600; //laua laius/kõrgus

    int w_ruut = wh/9;

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
    
    int laud[9][9] = //0 - pole määratud nrit
    {
        {5, 3, 4, 0, 7, 8, 0, 0, 2},
        {0, 7, 0, 1, 9, 5, 3, -4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {0, 5, 9, 7, 0, 0, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {0, 6, 0, 5, 3, 7, 0, 8, 4},
        {2, 8, 0, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };

    bool muudetav[9][9]; //kas sel kohal on number muudetav

    //std::cout << "aoo";

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


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            
            if(event.type == sf::Event::MouseButtonReleased){
                if (event.mouseButton.button == sf::Mouse::Left){
                    if(
                        event.mouseButton.x < 2*w_ruut && 
                        event.mouseButton.x > 1*w_ruut &&

                        event.mouseButton.y < 2*w_ruut && 
                        event.mouseButton.y > 1*w_ruut
                    ){
                        laud[6][6]++;
                        nrid[6][6].setString(std::to_string(laud[6][6]));
                    }
                }
            }
        }

        /*if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if(event.mouseButton.x < 2*w_ruut && 
            event.mouseButton.x > 1*w_ruut &&

            event.mouseButton.y < 2*w_ruut && 
            event.mouseButton.y > 1*w_ruut){
                laud[9][9]++;
                nrid[9][9].setString(std::to_string(laud[9][9]));
            }else{
                nrid[3][3].setString("aaa");
            }
        }*/

        window.clear();
        //laua taust
        for (int i = 0; i < 20; i++)
        {
            window.draw(trellid[i], 2, sf::Lines);
        }
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