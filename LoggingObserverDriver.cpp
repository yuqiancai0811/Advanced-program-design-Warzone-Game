#include "LoggingObserver.h"
#include "GameEngine.h"

void testLoggingObserver()
{
    /// Create a log observer
    LogObserver *logger = new LogObserver("gamelog.txt");

    //-----CommandProcessor::saveCommand() and Command::saveEffect() write in a log file---------------

    CommandProcessor cmdProcessor;
    cmdProcessor.Attach(logger);
    Command *loadMapCmd = new Command("Loadmap");
    loadMapCmd->saveEffect("Loaded map successfully");
    cmdProcessor.saveCommand(loadMapCmd);

    Command *validateMapCmd = new Command("Validatemap");
    validateMapCmd->saveEffect("Map validated successfully");
    cmdProcessor.saveCommand(validateMapCmd);

    Command *addPlayerCmd = new Command("Addplayer");
    addPlayerCmd->saveEffect("Player added successfully");
    cmdProcessor.saveCommand(addPlayerCmd);

    Command *gameStartCmd = new Command("Gamestart");
    gameStartCmd->saveEffect("Game started");
    cmdProcessor.saveCommand(gameStartCmd);


    // ---------GameEngine::transition() write in a log file--------------------
    GameEngine gameEngine;
    gameEngine.Attach(logger);

    gameEngine.transition("Start");
    gameEngine.transition("maploaded");
    gameEngine.transition("mapvalidated");
    gameEngine.transition("exit program");

    //--------------------Order::execute() and OrderList::addOrder() write in a log file------------------------------------

    vector<string> adjTerritoryNames1 = {"Territory 2", "Territory 3"}; // Storing the strings
    Territory *territory1 = new Territory("Territory 1", 0, 0, "Continent 1", adjTerritoryNames1);

    vector<string> adjTerritoryNames2 = {"Territory 1", "Territory 3"};
    Territory *territory2 = new Territory("Territory 2", 1, 1, "Continent 1", adjTerritoryNames2);

    vector<string> adjTerritoryNames3 = {"Territory 1", "Territory 2"};
    Territory *territory3 = new Territory("Territory 3", 2, 2, "Continent 2", adjTerritoryNames3);

    // Create a player and assign territories to the player:
    Player *player1 = new Player("Player 1");
    player1->addTerritory(territory1);
    player1->addTerritory(territory2);
    player1->setNumberOfReinforcement(1000);
    Card *airlift = new Card("Airlift");
    Card *blockade = new Card("BLOCKADE");

    player1->getHand().addCard(airlift);
    player1->getHand().addCard(blockade);

    territory1->setArmies(100);
    territory2->setArmies(100);
    territory3->setArmies(1000);

    Player *player2 = new Player("Player 2");
    player2->addTerritory(territory3);
    player2->setNumberOfReinforcement(1000);

    // Actual connections between territory objects:
    territory1->addAdjacentTerritory(territory2);
    territory2->addAdjacentTerritory(territory3);
    territory3->addAdjacentTerritory(territory2);

    territory1->setOwner("Player1");
    territory2->setOwner("Player1");
    territory3->setOwner("Player2");

    deployOrder *deploy_order = new deployOrder(100, territory1, player1);
    airliftOrder *airlift_order = new airliftOrder(100, territory1, territory3, player1);
    bombOrder *bomb_order = new bombOrder(territory3, player1);
    blockadeOrder *blockade_order = new blockadeOrder(100, player1, player2, territory2);
    negotiateOrder *negotiate_order = new negotiateOrder(player1, player2);


    //execute part of orders
    deploy_order->execute();
    airlift_order->execute();
    bomb_order->execute();
    // blockade_order->execute();
    // negotiate_order->execute();
    // advance_order->execute();

    // create an orderlist then addOrder()
    orderList ordersList;
    ordersList.Attach(logger);
    ordersList.addOrder(deploy_order);
    ordersList.addOrder(bomb_order);
    ordersList.addOrder(blockade_order);
    ordersList.addOrder(negotiate_order);
    
    std::cout << "Logging completed. Check gamelog.txt for results." << std::endl;
}

int main()
{
    testLoggingObserver();
    return 0;
}
