#include "../include/menu.h"
#include "../include/store.h"
#include "../include/utils.h"

// private:

// Individual report methods
// stock report
void Menu::generateStockReport(SalesReport &report)
{
    int titleLength = 12;
    clearConsole();
    setColor(YELLOW);
    cout << "Stock report";
    setColor(RESET);
    limh(REPORTS_DASH - titleLength);

    report.generateStockReport();

    pause();
}

// sales by product report
void Menu::generateSalesReportByProduct(SalesReport &report)
{
    int titleLength = 26;
    clearConsole();
    setColor(YELLOW);
    cout << "Sales Report by Product";
    setColor(RESET);
    limh(STOCK_DASH - titleLength);

    string productName;
    cout << "Enter product name: ";
    getline(cin, productName);

    if (!productName.empty())
    {
        report.generateSalesReportByProduct(productName);
    }
    else
    {
        cout << "Product name cannot be empty!" << endl;
    }

    pause();
}

// complete sales report
void Menu::generateCompleteSalesReport(SalesReport &report)
{
    int titleLength = 21;
    clearConsole();
    setColor(YELLOW);
    cout << "Complete Sales Report";
    setColor(RESET);
    limh(REPORTS_DASH - titleLength);

    int mostSold = report.getMostSoldProductByQuantity();
    int leastSold = report.getLeastSoldProductByQuantity();
    double mostSoldProfit = report.getMostSoldProductTotalSales() * (0.3 / 1.3); // calculating profit based on total sales already without tax and 30% margin
    // Sale Price = Cost × 1.3  →  Cost = Sale / 1.3
    // Profit = Sale - Cost = Sale - (Sale / 1.3) = Sale × (0.3 / 1.3)
    int topClient = report.getTopClientByValue();

    // Check if we have valid data
    if (mostSold == -1 || leastSold == -1 || topClient == -1)
    {
        cout << "No sales data available to generate complete report." << endl;
        pause();
        return;
    }

    // Pointer lookups
    Client *topClientObj = store.findClientById(topClient);
    Stock *mostSoldStock = store.findStockById(mostSold);
    Stock *leastSoldStock = store.findStockById(leastSold);

    // Display results with null checks
    cout << "Most sold product by quantity is: ";
    if (mostSoldStock != nullptr)
    {
        cout << mostSoldStock->getProductName() << " (ID: " << mostSold << ")" << endl;
    }
    else
    {
        cout << "Unknown (ID: " << mostSold << ")" << endl;
    }

    cout << "Least sold product by quantity is: ";
    if (leastSoldStock != nullptr)
    {
        cout << leastSoldStock->getProductName() << " (ID: " << leastSold << ")" << endl;
    }
    else
    {
        cout << "Unknown (ID: " << leastSold << ")" << endl;
    }

    cout << "Most sold product profit: " << fixed << setprecision(2) << mostSoldProfit << " eur." << endl;

    cout << "Top client by value is: ";
    if (topClientObj != nullptr)
    {
        cout << topClientObj->getName() << " (ID: " << topClient << ")" << endl;
    }
    else
    {
        cout << "Unknown (ID: " << topClient << ")" << endl;
    }
    limh(REPORTS_DASH);
    pause();
}

// most sold product by quantity
void Menu::showMostSoldProduct(SalesReport &report)
{
    int titleLength = 17;
    clearConsole();
    setColor(YELLOW);
    cout << "Most sold product";
    setColor(RESET);
    limh(REPORTS_DASH - titleLength);

    int mostSold = report.getMostSoldProductByQuantity();
    int quantity = report.getProductTotalQuantitySold(mostSold);
    double sales = report.getProductTotalSales(mostSold);
    Stock *mostSoldStock = store.findStockById(mostSold);

    // Check if we have valid data
    if (mostSold == -1)
    {
        cout << "No sales data available to generate report." << endl;
        pause();
        return;
    }

    cout << "Product ID: " << mostSold << endl;
    cout << "Product Name: " << mostSoldStock->getProductName() << endl;
    cout << "Total quantity sold: " << quantity << endl;
    cout << "Total sales value: " << fixed << setprecision(2) << sales << " eur." << endl;

    limh(REPORTS_DASH);
    pause();
}

// least sold product by quantity
void Menu::showLeastSoldProduct(SalesReport &report)
{
    int titleLength = 18;
    clearConsole();
    setColor(YELLOW);
    cout << "Least sold product";
    setColor(RESET);
    limh(REPORTS_DASH - titleLength);

    int leastSold = report.getLeastSoldProductByQuantity();
    int quantity = report.getProductTotalQuantitySold(leastSold);
    double sales = report.getProductTotalSales(leastSold);
    Stock *leastSoldStock = store.findStockById(leastSold);

    if (leastSold == -1)
    {
        cout << "No sales data available to generate report." << endl;
        pause();
        return;
    }

    cout << "Product ID: " << leastSold << endl;
    cout << "Product Name: " << leastSoldStock->getProductName() << endl;
    cout << "Total quantity sold: " << quantity << endl;
    cout << "Total sales value: " << fixed << setprecision(2) << sales << " eur." << endl;

    limh(REPORTS_DASH);
    pause();
}

// top client by value sold
void Menu::showTopClient(SalesReport &report)
{
    int titleLength = 24;
    clearConsole();
    setColor(YELLOW);
    cout << "Top client by value sold";
    setColor(RESET);
    limh(REPORTS_DASH - titleLength);

    int topClient = report.getTopClientByValue();
    double totalPurchases = report.getClientTotalPurchases(topClient);
    Client *topClientObj = store.findClientById(topClient);

    if (topClient == -1)
    {
        cout << "No sales data available to generate report." << endl;
        pause();
        return;
    }

    cout << "Client ID: " << topClient << endl;
    cout << "Client Name: " << topClientObj->getName() << endl;
    cout << "Total purchases: " << fixed << setprecision(2) << totalPurchases << " eur." << endl;

    limh(REPORTS_DASH);

    pause();
}

// SubMenus - management:
void Menu::printProducts()
{
    // just for pretty formatting
    int titleLength = 13;

    clearConsole();
    setColor(YELLOW);
    cout << "Products View";
    setColor(RESET);
    limh(PRODUCTS_DASH - titleLength); // horizontal limit with dashes

    setColor(CYAN);
    cout << "ID | Product Name           | Quantity | Price w/Tax (eur)" << endl;
    setColor(RESET);
    limh(PRODUCTS_DASH);

    for (const Stock &item : store.getShelf())
    {
        if (item.getQuantity() == 0)
            setColor(RED); // red for zero quantity

        stringstream ss;
        cout << setw(2) << right << item.getStockId() << " | "
             << setw(22) << left << item.getProductName() << " | "
             << setw(8) << right << item.getQuantity() << " | "
             << fixed << setprecision(2) << item.getSaleValue() * 1.23 << " eur"
             << endl;

        setColor(RESET); // resets color
    }

    limh(PRODUCTS_DASH);
}

// Search stock
void Menu::searchStock()
{
    bool run;
    do
    {
        clearConsole();
        printStock("Stock View");

        string name;
        setColor(Cyan);
        cout << "Stock Search Menu" << endl;
        setColor(RESET);
        limh(STOCK_DASH);
        cout << "Please enter a product name to search in stock" << endl;
        cout << "Name: ";
        getline(cin, name);

        vector<Stock> items = store.searchPurchaseStock(name);
        run = Menu::showSearch(items);
    } while (run);
}

// Search stock results
bool Menu::showSearch(vector<Stock> &matchedItems)
{
    clearConsole();
    setColor(YELLOW);
    cout << "Search Results:" << endl;
    setColor(RESET);
    if (matchedItems.empty())
    {
        limh();
        setColor(CYAN);
        cout << "No matching results found" << endl;
        setColor(RESET);
        limh();
    }
    else
    {
        setColor(Magenta);
        cout << "Found " << matchedItems.size() << " matching results:" << endl;
        setColor(RESET);
        limh();

        setColor(CYAN);
        cout << "ID | Product Name           | Quantity | Cost eur" << endl;
        setColor(RESET);
        limh();

        for (const Stock &item : matchedItems)
        {
            if (item.getQuantity() == 0)
                setColor(RED); // red for zero quantity

            cout << item.toDisplay() << endl;

            if (item.getQuantity() == 0)
                setColor(RESET); // resets color
        }
        limh();
    }

    // default NO
    if (promptyesOrNO("Do you wish to keep searching?"))
        return true;
    else
        return false;
}

// Add stock
void Menu::addStock()
{
    vector<int> idColor; // saves ids for marking when changed

    do
    {
        clearConsole();
        printStock("Stock View", &idColor, Green);

        Stock item;
        stringstream line;
        string field;

        setColor(Cyan);
        cout << "Add Stock Menu" << endl;
        setColor(RESET);
        limh(STOCK_DASH);
        cout << "Product Name (leave empty to go back)" << endl;
        cout << "Name: ";

        getline(cin, field);
        if (field.empty())
            return;
        if (field.length() > STRING_PRODUCT_MAX) // to ensure name fits in tightly with the menu structure
        {
            cout << "Please enter a shorter name." << endl;
            pause();
            break;
        }

        item.setProductName(field);

        string name = item.getProductName();
        Stock *findItem = store.findStockByName(name);
        if (findItem != nullptr) // if product found
        {
            // default yes
            if (promptYESOrNo("Product Name was found in stockpile, do you want to add to product quantity?"))
            {
                int addedQuantity = getValidatedInt("Quantity to add: ");
                store.changeQuantityStock(findItem, addedQuantity + findItem->getQuantity());
                cout << endl
                     << "Item ID: " << findItem->getStockId() << " changed in stockpile!" << endl;
            }
        }
        else // if not found
        {
            item.setStockId(Stock::getNextStockId());
            Stock::incrementStockId();
            item.setQuantity(getValidatedInt("Quantity: "));
            item.setCostValue(getValidatedDouble("Cost Value: "));

            store.addPurchaseStock(&item);
        }
        idColor.push_back((item.getStockId()));

        // default no
    } while (promptyesOrNO("Do you want to register another item?"));
}

// Edit stock
void Menu::editStock()
{
    vector<int> idColor; // saves ids for marking when changed
    while (true)
    {
        clearConsole();
        printStock("Change Item Menu", &idColor, Green);

        string prompt;
        int id;
        cout << "Please enter the ID of the product you wish to change (Enter 0 to return)" << endl;
        cout << "ID: ";
        getline(cin, prompt);

        if (!validateMenuInput(prompt, id))
            continue;

        if (id <= 0) // go back in menu
            return;

        Stock *item = store.findStockById(id);
        if (item == nullptr)
        {
            cout << "Item not found in stock" << endl;
            pause();
            continue;
        }
        clearConsole();
        printStock("Stock View");

        setColor(Cyan);
        cout << "Changing product: " << item->getStockId() << "-" << item->getProductName() << endl;
        setColor(RESET);
        limh();

        if (!promptYESOrNo("Do you wish to proceed?"))
        {
            continue;
        }
        limh(STOCK_DASH);

        stringstream itemString;
        itemString << item->getStockId() << ",";
        cout << "Enter new product data" << endl;
        cout << "Product name: ";
        getline(cin, prompt);
        itemString << prompt << ",";
        itemString << getValidatedInt("Quantity: ") << ",";
        itemString << getValidatedDouble("Cost: ");

        Stock newItem;
        newItem.fromString(itemString.str());
        store.changePurchaseStock(item, newItem);
        cout << "Stock Updated!" << endl;

        idColor.push_back(item->getStockId());

        if (!promptyesOrNO("Do you wish to keep editing items?"))
            return;
    }
}

// Remove stock
void Menu::removeStock()
{
    vector<int> idColor; // saves ids for marking when changed
    while (true)
    {
        string prompt;
        int id;
        do
        {
            printStock("Remove Item Menu", &idColor, Yellow);

            limh(STOCK_DASH);

            cout << "Please enter the ID of the product you wish to remove (Enter 0 to return)" << endl;
            cout << "ID: ";
            getline(cin, prompt);

        } while (!validateMenuInput(prompt, id));
        if (id <= 0) // go back in menu
            return;

        Stock *item = store.findStockById(id);
        if (item == nullptr)
        {
            cout << "Item not found in stock" << endl;
            pause();
            continue;
        }
        if (item->getQuantity() == 0)
        {
            cout << "Item quantity already empty" << endl;
            pause();
            continue;
        }

        setColor(Cyan);
        cout << "Removing product: " << item->getStockId() << "-" << item->getProductName() << endl;
        setColor(RESET);
        if (!promptYESOrNo())
        {
            continue;
        }
        limh(STOCK_DASH);

        store.removePurchaseStock(item);
        cout << "Stock Updated!" << endl;

        if (!promptyesOrNO("Do you wish to keep removing items?"))
            return;
    }
}

// SubMenus - shopping:
void Menu::printCart()
{
    int titleDASH = CART_DASH - 9; // to make sure it fits the rest of the horizontal lims
    clearConsole();
    setColor(YELLOW);
    cout << "Your cart";
    setColor(RESET);

    limh(titleDASH);
    setColor(CYAN);
    cout << setw(2) << "ID" << " | "
         << setw(22) << left << "Product Name" << " | "
         << setw(5) << right << "Qtty" << " | "
         << setw(12) << right << "Price" << " | "
         << setw(12) << right << "Total"
         << endl;
    setColor(RESET);
    limh(CART_DASH);
    if (store.getCart().empty())
    {
        cout << "Cart is empty." << endl;
        limh(CART_DASH);
        return;
    }
    for (const CartItem &cartItem : store.getCart())
    {
        cout << cartItem.toDisplay() << endl;
    }

    limh(CART_DASH);
}

// Add product to cart
void Menu::addProductCart()
{
    while (true)
    {
        // prints visible menu

        clearConsole();
        if (menuState == false)
            printProducts();
        else
            printCart();

        int id = getValidatedInt("Insert product ID to add to cart (Enter 0 to return): ", true);
        if (id == 0)
            return;

        // checks initial quantity in shelf and its existence
        Stock *item = store.findShelf(id);
        if (item == nullptr)
        {

            cout << endl
                 << "Try another ID" << endl;
            pause();
            continue;
        }
        if (item->getQuantity() == 0)
        {
            cout << endl
                 << "Out of Stock. " << endl;
            pause();
            continue;
        }

        /*verify item quantity is valid*/
        // needs cycle to ensure cartitem is created, or function is exited
        int quantity = getValidatedInt("How many do you want good sir? (Enter 0 to return): ");
        string input;

        CartItem *bagged_item = store.findItemCart(item->getStockId());
        if (bagged_item != nullptr) // item exists in cart
        {

            // checking if input quantity doesn't exceed quantity in shelf
            if (item->getQuantity() >= quantity)
            {
                bagged_item->setQuantity(bagged_item->getQuantity() + quantity); // add the input quantity to existing quantity
                item->setQuantity(item->getQuantity() - quantity);               // remove the input quantity from shelf
            }
            else // quantity exceeds, ask user if it should add all remaining quantity anyway
            {
                cout << "Quantity asked exceeds quantity in stock."
                     << endl;

                stringstream ss;
                ss << "Do you wish to buy all " << item->getQuantity() << " existing items instead?";
                if (promptYESOrNo(ss.str()))
                {
                    bagged_item->setQuantity(item->getQuantity()); // if confirmed will add all existing stock items to cart
                    item->setQuantity(0);                          // as we add to cart, we remove from shelf
                }
            }
        }
        else // bagged item was not found (nullptr) and needs to be created
        {
            // checks again if input quantity doesn't exceed stock quantity
            if (item->getQuantity() >= quantity)
            {
                CartItem newItemCart(*item, quantity);  // create the new bagged item
                store.getCart().push_back(newItemCart); // add the new bagged item the vector cart
                item->setQuantity(item->getQuantity() - quantity);
            }
            else // if exceeds
            {
                cout << "Quantity asked exceeds quantity in stock."
                     << endl;

                stringstream ss;
                ss << "Do you wish to buy all " << item->getQuantity() << " existing items instead?";
                if (promptYESOrNo(ss.str()))
                {
                    CartItem newItemCart(*item, item->getQuantity()); // create the new bagged item
                    store.getCart().push_back(newItemCart);           // add the new bagged item the vector cart
                    item->setQuantity(0);
                }
            }
        }

        cout << "Cart Updated!" << endl;

        if (promptyesOrNO("Do you wish to continue adding?"))
            continue;
        else
            return;
    }
}

// Remove product from cart
void Menu::removeProductCart()
{
    while (true)
    {
        clearConsole();

        if (menuState == false)
            printProducts();
        else
            printCart();
        int id = getValidatedInt("Insert product ID to remove (Enter 0 to return): ", true);
        if (id == 0)
            return;

        int index;
        CartItem *bagged_item = store.findItemCart(id, &index);
        Stock *item = store.findStockById(id);

        string input;
        if (bagged_item != nullptr)
        {
            store.getCart().erase(store.getCart().begin() + index);
            item->setQuantity(item->getQuantity() + bagged_item->getQuantity());

            cout << "Product removed from cart." << endl;
            if (!promptyesOrNO("Do you wish to continue removing?"))
            {
                break;
            }
        }
        else
        {
            cout << "Item not found." << endl;
            if (!promptyesOrNO("Do you wish to continue removing?"))
            {
                break;
            }
        }
    }
}

// Change quantity of product in cart
void Menu::changeProductCart()
{
    while (true)
    {

        clearConsole();
        if (menuState == false)
            printProducts();
        else
            printCart();

        int id = getValidatedInt("Insert product ID in cart to change (Enter 0 to return): ", true);
        if (id == 0)
            return;

        CartItem *bagged_item = store.findItemCart(id);
        if (bagged_item) // if item exists in cart
        {

            Stock *item = store.findStockById(id);

            int item_quantity = item->getQuantity();
            int bagged_quantity = bagged_item->getQuantity();
            int quantity = getValidatedInt("Insert new quantity: ", false);
            quantity = quantity - bagged_item->getQuantity(); // will be the added/removed quantity from either cart or shelf

            if (quantity > item->getQuantity())
            {
                cout << "Not enough stock."
                     << endl;

                stringstream ss;
                ss << "Do you wish to buy all " << item->getQuantity() << " remaining items instead? ";
                if (promptYESOrNo(ss.str()))
                {
                    bagged_item->setQuantity(item_quantity + bagged_quantity);
                    item->setQuantity(0);
                }
                else
                    return;
            }
            else
            {
                bagged_item->setQuantity(quantity + bagged_quantity);
                item->setQuantity(item_quantity - quantity);
            }
            cout << "Product quantity changed." << endl;
            if (!promptyesOrNO("Do you want to continue adjusting? "))
                return;
        }
        else // if it doesn't exist in cart, refuse to update
        {
            string input;
            cout << "Product not found in cart. Please select a valid ID" << endl;
            if (!promptyesOrNO("Do you want to continue adjusting? "))
                return;
        }
    }
}

// Checkout menu
void Menu::checkoutMenu()
{
    if (store.getCart().empty())
    {
        cout << "Cart is empty." << endl;
        pause();
        return;
    }

    // Display cart and calculate total
    clearConsole();
    cout << "Your cart:" << endl;
    printCart();

    double total = store.calculateCartTotal();
    cout << "Total: " << total << " eur" << endl;

    if (!promptYESOrNo("Do you wish to confirm?"))
        return;

    // Handle client selection/creation
    Client *selectedClient = handleClientSelection();

    // Process payment
    double payment = processPayment(total);
    if (payment < 0) // User cancelled payment
        return;

    // Complete checkout
    completeCheckout(selectedClient, payment, total);
}

// Process payment
double Menu::processPayment(double total)
{
    double payment = getValidatedDouble("Insert payment amount: ");

    while (payment < total)
    {
        cout << "Nice joke, that's not enough." << endl;
        if (promptYESOrNo("Do you want to try again?"))
            payment = getValidatedDouble("Insert payment amount: ");
        else
            return -1; // Signal cancellation
    }

    return payment;
}

// Complete checkout
void Menu::completeCheckout(Client *client, double payment, double total)
{
    clearConsole();

    // Apply gambling only for registered clients
    if (client && client->getClientId() != 0)
    {
        gambling(store.getCart(), 25);
    }

    // Create receipt with appropriate client ID
    int clientId = client ? client->getClientId() : 0;
    Receipt receipt(store.getCart(), payment, clientId);

    cout << receipt.toDisplay();

    // Update stock and clear cart and save receipt
    store.addReceipt(receipt);
    FileManager::saveReceipts(store.getSalesList());
    store.updateStockFromShelf();
    store.clearCart();

    pause();
}

// Gambling for free products
void Menu::gambling(vector<CartItem> &sale, int chance)
{
    srand(time(0));
    int badLuck = (rand() % 101); // badluck goes from 0 to 100

    if (badLuck < chance) // if the "bad luck" is less than the chance, win
    {

        srand(time(0));                         // reinitializing seed
        int sortedIndex = rand() % sale.size(); // randomizing index

        CartItem jackpot = sale.at(sortedIndex);
        cout << "You won a free " << jackpot.getProductName() << "!" << endl;
        jackpot.setQuantity(1); // client only gets one, >:(

        double fullPrice = jackpot.getSaleWithoutTax();
        jackpot.setSaleWithoutTax(-fullPrice); // Negate full price (as base)

        sale.push_back(jackpot); // item gets added at end of receipt for display
    }
}

//  SubMenus - logins:
// print clients
void Menu::printClients(const string &title, vector<int> *idColor, const string colorCode)
{
    clearConsole();
    int titleDASH = CLIENT_DASH - title.length(); // to make sure it fits the rest of the horizontal lims

    setColor(YELLOW);
    cout << title;
    setColor(RESET);
    limh(titleDASH);

    setColor(CYAN);
    cout << "ID | Client Name        | Address                             | Phone        | Status" << endl;
    setColor(RESET);
    limh(CLIENT_DASH);

    for (const Client &client : store.getClientList())
    {
        // Color inactive clients red
        if (!client.getActivity())
            setColor(RED);

        if (idColor != nullptr)
            for (int id : *idColor)
            {
                if (client.getClientId() == id)
                    setColor(colorCode); // <color> for when client matches vector idColor
            }

        cout << client.toDisplay() << endl;

        setColor(RESET); // resets color
    }

    limh(CLIENT_DASH);
}

// kill client
void Menu::killClient()
{
    vector<int> idColor; // saves ids for marking when changed
    while (true)
    {
        string prompt;
        int id;
        do
        {
            printClients("Kill Client Menu", &idColor, YELLOW);

            limh(CLIENT_DASH);

            cout << "Please enter the ID of the client you wish to deactivate (Enter 0 to return)" << endl;
            cout << "ID: ";
            getline(cin, prompt);

        } while (!validateMenuInput(prompt, id));
        if (id <= 0) // go back in menu
            return;

        Client *client = store.findClientById(id);
        if (client == nullptr)
        {
            cout << "Client not found" << endl;
            pause();
            continue;
        }
        if (!client->getActivity())
        {
            cout << "Client is already dead bro" << endl;
            pause();
            continue;
        }

        setColor(CYAN);
        cout << "Killing client: " << client->getClientId() << "-" << client->getName() << endl;
        setColor(RESET);
        if (!promptYESOrNo())
        {
            continue;
        }
        limh(CLIENT_DASH);

        client->setActivity(false);
        idColor.push_back(client->getClientId()); // mark this client as changed
        cout << "Client killed!" << endl;
        FileManager::saveClients(store.getClientList());

        if (!promptyesOrNO("Do you wish to keep killing clients?"))
            return;
    }
}

// revive client
void Menu::reviveClient()
{
    vector<int> idColor; // saves ids for marking when changed
    while (true)
    {
        string prompt;
        int id;
        do
        {
            printClients("Revive Client Menu", &idColor, GREEN);

            limh(CLIENT_DASH);

            cout << "Please enter the ID of the client you wish to reactivate (Enter 0 to return)" << endl;
            cout << "ID: ";
            getline(cin, prompt);

        } while (!validateMenuInput(prompt, id));
        if (id <= 0) // go back in menu
            return;

        Client *client = store.findClientById(id);
        if (client == nullptr)
        {
            cout << "Client not found" << endl;
            pause();
            continue;
        }
        if (client->getActivity())
        {
            cout << "Client is already alive and well" << endl;
            pause();
            continue;
        }

        setColor(CYAN);
        cout << "Reviving client: " << client->getClientId() << "-" << client->getName() << endl;
        setColor(RESET);
        if (!promptYESOrNo())
        {
            continue;
        }
        limh(CLIENT_DASH);

        client->setActivity(true);
        idColor.push_back(client->getClientId()); // mark this client as changed
        cout << "Client revived!" << endl;
        FileManager::saveClients(store.getClientList());

        if (!promptyesOrNO("Do you wish to keep reviving clients?"))
            return;
    }
}

// change client name
void Menu::changeClientName()
{
    vector<int> idColor; // saves ids for marking when changed
    while (true)
    {
        clearConsole();
        printClients("Change Client Name Menu", &idColor, GREEN);

        string prompt;
        int id;
        cout << "Please enter the ID of the client whose name you wish to change (Enter 0 to return)" << endl;
        cout << "ID: ";
        getline(cin, prompt);

        if (!validateMenuInput(prompt, id))
            continue;

        if (id <= 0) // go back in menu
            return;

        Client *client = store.findClientById(id);
        if (client == nullptr)
        {
            cout << "Client not found" << endl;
            pause();
            continue;
        }
        clearConsole();
        printClients("Client View");

        setColor(CYAN);
        cout << "Changing client name: " << client->getClientId() << "-" << client->getName() << endl;
        setColor(RESET);
        limh();

        if (!promptYESOrNo("Do you wish to proceed?"))
        {
            continue;
        }
        limh(CLIENT_DASH);

        cout << "Enter new client name" << endl;
        string clientName = getValidatedName();

        client->setName(clientName);
        cout << "Client name updated!" << endl;

        idColor.push_back(client->getClientId());
        FileManager::saveClients(store.getClientList());

        return;
    }
}

// handle client selection
Client *Menu::handleClientSelection()
{
    if (promptyesOrNO("Are you part of our extremely cool clientele?"))
    {
        cout << "Enter your name: ";
        string name;
        getline(cin, name);

        Client *client = store.findClientByName(name);
        if (client)
        {
            cout << "Welcome back, " << client->getName() << ", you son of your mother!" << endl;
            return client;
        }
        else
        {
            cout << "Client not found." << endl;
            if (promptyesOrNO("Would you like to register as a new client?"))
            {
                return registerClient();
            }
        }
    }
    else if (promptyesOrNO("But do you want to be part of the cool kids club?"))
    {
        return registerClient();
    }

    // Return default client (guest checkout)
    return nullptr; // We'll handle this in completeCheckout
}

// register new client
Client *Menu::registerClient()
{
    string clientName = getValidatedName();
    string clientAddress = getValidatedAddress();
    int clientContact = getValidatedContact();

    Client newClient(clientName, clientContact, clientAddress, true);
    store.addClient(newClient);
    FileManager::saveClients(store.getClientList());
    return &store.getClientList().back();
}

void Menu::printStock(const string &title, vector<int> *idColor, const string colorCode)
{
    clearConsole();
    int titleDASH = STOCK_DASH - title.length(); // to make sure it fits the rest of the horizontal lims

    setColor(YELLOW);
    cout << title;
    setColor(RESET);
    limh(titleDASH);

    setColor(CYAN);
    cout << "ID | Product Name           | Quantity | Cost (eur)" << endl;
    setColor(RESET);
    limh();

    for (const Stock &item : store.getStock())
    {
        if (item.getQuantity() == 0)
            setColor(RED); // red for zero quantity
        if (idColor != nullptr)
            for (int id : *idColor)
            {
                if (item.getStockId() == id)
                    setColor(colorCode); // <color> for when item matches vector idColor
            }

        cout << item.toDisplay() << endl;

        setColor(RESET); // resets color
    }

    limh();
}

// intresting constructor, stores a reference to the store and uses it
Menu::Menu(Store &storeReference) : store(storeReference) {}

// main menu
void Menu::main()
{
    // main loop
    // vars
    bool run = true;
    int opt;
    string input;
    // adding copy of stockList to shelf
    // this will display items to user in specified order:
    while (run)
    {
        do
        {
            clearConsole();
            setColor(Cyan);
            cout << "Shop menu" << endl;
            setColor(RESET);
            limh(STOCK_DASH);
            cout << "1. Shop Sales" << endl;
            limh(STOCK_DASH);
            cout << "2. Shop Stock" << endl;
            limh(STOCK_DASH);
            cout << "3. Reports" << endl;
            limh(STOCK_DASH);
            cout << "4. Clients" << endl;
            limh(STOCK_DASH);
            cout << "5. Exit" << endl;
            limh(STOCK_DASH);
            cout << "Option: ";
            getline(cin, input);
        } while (!validateMenuInput(input, opt));

        switch (opt)
        {
        case 1:
            // show products, give an option to buy and checkout or cancel
            shopping();
            break;
        case 2:
            // show stock, give an option to add or remove and cancel, among some other bonus features
            // bar access to stock if a sale is in process, in order to prevent mismanagement of stock and shelf items
            if (!store.getCart().empty())
            {
                cout << "You have products in cart! Clear the cart or finish checking out to proceed" << endl
                     << "Unable to access stock menu" << endl;
                pause();
                break;
            }
            management();
            break;
        case 3:
            reports();
            break;
        case 4:
            logins();
            break;
        case 5:
            // exit
            clearConsole();
            run = false;
            break;
        default:
            // user is a bit slow, what can we do
            cout << "Invalid input, try again." << endl;
            pause();
            break;
        }
    }
    return;
}

// menu for sales
void Menu::shopping()
{
    string input;
    int opt;
    while (true)
    {
        do
        {
            clearConsole();

            // Depending on the state of the menu
            // will print either the products in stock or in cart
            //   menuState:
            //   false -> Shows Products (default)
            //   true -> Shows Cart
            int limiterType;
            string switchName;
            if (menuState == false)
            {
                // needs to print price for client, meaning, profit margin + maybe with tax
                printProducts();
                limiterType = PRODUCTS_DASH;
                switchName = "4. View Cart";
            }
            else
            {
                printCart();
                limiterType = CART_DASH;
                switchName = "4. View Products";
            }

            setColor(Cyan);
            cout << "Sales Options:" << endl;
            setColor(RESET);
            limh(limiterType);
            cout << setw(limiterType - 20) << left << "1. Add to cart" << setw(20) << left << switchName << endl;
            limh(limiterType);
            cout << setw(limiterType - 20) << left << "2. Change cart" << setw(20) << left << "5. Clear cart" << endl;
            limh(limiterType);
            cout << setw(limiterType - 20) << left << "3. Remove from cart" << setw(20) << left << "6. Checkout" << endl;
            limh(limiterType);
            cout << "0. Go back" << endl;
            limh(limiterType);
            cout << "Option: ";

            getline(cin, input);
        } while (!validateMenuInput(input, opt));

        switch (opt)
        {
        case 1:
            addProductCart();
            break;
        case 2:
            changeProductCart();
            break;
        case 3:
            removeProductCart();
            break;
        case 4:
            menuState = !menuState; // flips menuState
            break;
        case 5:
            store.clearCart();
            break;
        case 6:
            checkoutMenu();
            break;
        case 0:
            return;

        default:
            cout << "Invalid input, try again." << endl;
            pause();
            break;
        }
    };
}

// menu for stock
void Menu::management()
{
    string input;
    int opt;
    while (true)
    {
        do
        {
            clearConsole();
            printStock("Stock View");

            setColor(Cyan);
            cout << "Stock Editing Menu" << endl;
            setColor(RESET);
            limh(STOCK_DASH);
            cout << "1. Search Stock" << endl;
            limh(STOCK_DASH);
            cout << "2. Add Stock" << endl;
            limh(STOCK_DASH);
            cout << "3. Change Stock" << endl;
            limh(STOCK_DASH);
            cout << "4. Remove Stock" << endl;
            limh(STOCK_DASH);
            cout << "0. Go Back" << endl;
            limh(STOCK_DASH);
            cout << "Option: ";
            getline(cin, input);
        } while (!validateMenuInput(input, opt));

        switch (opt)
        {
        case 1:
            searchStock();
            break;
        case 2:
            addStock();
            break;
        case 3:
            editStock();
            break;
        case 4:
            removeStock();
            break;
        case 0:
            return;
        default:
            cout << "Invalid input, try again." << endl;
            pause();
            break;
        }
    }
}

// menu for reports
void Menu::reports()
{
    string input;
    int opt;

    // Initialize report
    SalesReport report;
    report.initialize(store.getSalesList(), store.getStock());

    while (true)
    {
        do
        {
            clearConsole();

            setColor(Cyan);
            cout << "Sales Reports Menu" << endl;
            setColor(RESET);
            limh(REPORTS_DASH);
            cout << "1. Total stock value" << endl;
            limh(REPORTS_DASH);
            cout << "2. Sales Report by Product" << endl;
            limh(REPORTS_DASH);
            cout << "3. Complete Sales Report" << endl;
            limh(REPORTS_DASH);
            cout << "4. Most Sold Product" << endl;
            limh(REPORTS_DASH);
            cout << "5. Least Sold Product" << endl;
            limh(REPORTS_DASH);
            cout << "6. Top Client by Value" << endl;
            limh(REPORTS_DASH);
            cout << "0. Go Back" << endl;
            limh(REPORTS_DASH);
            cout << "Option: ";
            getline(cin, input);
        } while (!validateMenuInput(input, opt));

        switch (opt)
        {
        case 1:
            generateStockReport(report);
            break;
        case 2:
            generateSalesReportByProduct(report);
            break;
        case 3:
            generateCompleteSalesReport(report);
            break;
        case 4:
            showMostSoldProduct(report);
            break;
        case 5:
            showLeastSoldProduct(report);
            break;
        case 6:
            showTopClient(report);
            break;
        case 0:
            return;
        default:
            cout << "Invalid input, try again." << endl;
            pause();
            break;
        }
    }
}

// menu for clients
void Menu::logins()
{
    while (true)
    {
        string input;
        int opt;

        do
        {
            clearConsole();
            printClients("Clients View");
            setColor(Cyan);
            cout << "Clients menu" << endl;
            setColor(RESET);
            limh(CLIENT_DASH);
            cout << "1. Register new client" << endl;
            limh(CLIENT_DASH);
            cout << "2. Kill client" << endl;
            limh(CLIENT_DASH);
            cout << "3. Revive client" << endl;
            limh(CLIENT_DASH);
            cout << "4. Change client name" << endl;
            limh(CLIENT_DASH);
            cout << "5. Exit" << endl;
            limh(CLIENT_DASH);
            cout << "Option: ";
            getline(cin, input);
        } while (!validateMenuInput(input, opt));

        switch (opt)
        {
        case 1:
            registerClient();
            break;
        case 2:
            killClient();
            break;
        case 3:
            reviveClient();
            break;
        case 4:
            changeClientName();
            break;
        case 5:
            return;

        default:
            break;
        }
    }
}