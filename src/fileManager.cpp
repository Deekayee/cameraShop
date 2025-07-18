#include "../include/fileManager.h"

#include "../include/receipt.h"

#include <iostream>
#include <fstream>
#include <array>

// Function to load stock from a file
void FileManager::loadStock(vector<Stock> &stockList)
{
    fstream file("../data/stockList.csv", ios::in);
    if (!file.is_open())
    {
        fstream file("../data/stockList.csv", ios::app);

        // gives the file a header
        cout << "Creating file - stockList.csv..." << endl;
        file << "StockId,ProductName,Quantity,CostValue" << endl;
    }

    string line;
    getline(file, line); // ignore header
    int maxId = 0;
    while (getline(file, line))
    {
        Stock item;
        item.fromString(line);
        stockList.push_back(item);

        // Track the highest ID
        if (item.getStockId() > maxId)
            maxId = item.getStockId();
    }

    // Set the nextStockId to one greater than the highest existing ID
    Stock::setNextStockId(maxId + 1);

    file.close();
    return;
}

// Function to save stock to a file
bool FileManager::saveStock(const vector<Stock> &stockList)
{
    ofstream file("../data/stockList.csv");
    if (!file.is_open())
    {
        cout << "Error opening file: " << "../data/stockList.csv" << endl;
        return false;
    }

    file << "StockId,ProductName,Quantity,CostValue" << endl;
    for (const Stock &item : stockList)
    {
        file << item.toString() << endl;
    }

    file.close();
    return true;
}

// Function to load clients from a file
void FileManager::loadClients(vector<Client> &clientList)
{
    fstream file("../data/clientList.csv", ios::in);
    if (!file.is_open())
    {
        fstream file("../data/clientList.csv", ios::app);

        // gives the file a header
        cout << "Creating file - receiptList.csv..." << endl;
        file << "ClientId,Name,Contact,Address" << endl;
    }

    string line;
    getline(file, line);
    int maxId = 0;

    while (getline(file, line))
    {
        Client client;
        client.fromString(line);
        clientList.push_back(client);

        // Track the highest ID
        if (client.getClientId() > maxId)
            maxId = client.getClientId();
    }

    // Set the nextStockId to one greater than the highest existing ID
    Client::setNextId(maxId + 1);

    file.close();
    return;
}

// Function to save clients to a file
bool FileManager::saveClients(const vector<Client> &clientList)
{
    ofstream file("../data/clientList.csv");
    if (!file.is_open())
    {
        cout << "Error opening file: " << "../data/clientList.csv" << endl;
        return false;
    }

    file << "ClientId,Name,Contact,Address,IsActive" << endl;
    for (const Client &client : clientList)
    {
        file << client.toString() << endl;
    }

    file.close();
    return true;
}

// Function to load receipts from a file
void FileManager::loadReceipts(array<Receipt, 100> &saleList)
{
    fstream file("../data/receiptList.csv", ios::in);
    if (!file.is_open())
    {
        fstream file("../data/receiptList.csv", ios::app);

        // gives the file a header
        cout << "Creating file - receiptList.csv..." << endl;
        file << "ReceiptId,ClientId,PaymentAmount,Date,Items" << endl;
    }

    string line;
    getline(file, line); // ignore header
    int maxId = 0;
    int i = 0; // Initialize i to 0

    while (getline(file, line) && i < 100) // Add bounds check
    {
        if (!line.empty()) // Check if line is not empty
        {
            Receipt sale;
            sale.fromString(line);
            saleList.at(i) = sale;

            // Track the highest ID
            if (sale.getReceiptId() > maxId)
                maxId = sale.getReceiptId();

            i++; // Increment i
        }
    }

    // Set the nextReceiptId to one greater than the highest existing ID
    Receipt::setNextReceiptId(maxId + 1);

    file.close();
    return;
}

// Function to save receipts to a file
bool FileManager::saveReceipts(const array<Receipt, 100> &saleList)
{
    ofstream file("../data/receiptList.csv");
    if (!file.is_open())
    {
        cout << "Error opening file: " << "../data/receiptList.csv" << endl;
        return false;
    }

    file << "ReceiptId,ClientId,PaymentAmount,Date,Items" << endl;

    for (int i = 0; i < 100; i++)
    {
        if (saleList.at(i).getReceiptId() > 0) // Only save valid receipts
        {
            file << saleList.at(i).toString() << endl;
        }
    }

    file.close();
    return true;
}