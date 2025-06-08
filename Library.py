#Joshua O. Pagcaliwagan CMSC12 T-4L The code revolves around making a Library inventory system in Python. This system should be able to store information about the books in the library, maintain a borrower’s list per book, and lastly maintain a log of transactions that have happened in the system.

#Dictionary for storing books and their respective information
booksDict = {}

#Function to add a book
def addBook():
    #Ask user for the information about the book(Title, Author, Date Published)
    title = input("(๑ > ᴗ < ๑) Hey kitty! Can you put the title of the book you want to add: ")
    author = input("(๑ > ᴗ < ๑) Oh, can you also input its author: ")
    datePublished = input("(๑ > ᴗ < ๑) And lastly, when it was published (e.g. 20 Dec 2023): ")

    # Produce Book ID
    bookID = createBookID()

    # Append book information
    booksDict[bookID] = {
        'Book ID': bookID,
        'Title': title,
        'Author': author,
        'Date Published': datePublished,
        #Status is initialized to “Available”
        'Status': 'Available',
        #List of borrowers is initialized to empty
        'List of Borrowers': []
    }
    print(f"(∩˃o˂∩)♡ Yehey! We have added the book '{title}' by {author} with the book ID '{bookID}'.")
#global counter to check number of books
bookCounter = 0

#Function to create book ID
def createBookID():
    global bookCounter
    bookCounter += 1
    bookID = f"B{bookCounter}"
    return bookID

#Function to delete a book
def deleteBook():
    #Ask user for the Title and Author of the book they want to delete
    title = input("(っ◞‸◟ c) Aww, why you have to delete? Huhuhu, fine, input the title: ")
    author = input("(っ◞‸◟ c) Also, input its author: ")

    # Find the book by title and author
    for bookID, bookInfo in booksDict.items():
        if bookInfo['Title'] == title and bookInfo['Author'] == author:
            
            #If found, delete the book
            del booksDict[bookID]
            print(f"૮(˶╥︿╥)ა Nooo! Huhuhu, the book '{title}' by {author} has been sadly deleted.")
            break
    #If book is not found, print a message.
    else:
        print("(ᴗ_ ᴗ。) Oh, sorry, but that book can't be found. ")

#Function to delete all books
def deleteAllBooks():
    # Check if there are books to delete
    if not booksDict:
        print("(¬_¬) Come on, there aren't any books to delete.")
        return
    #Asks for confirmation before deleting all entries in the Books Dictionary
    assurance = input("(｡•́︿•̀｡) Are you really sure you want to delete all books? You're gonna miss them. Input yes or no: ")
    if assurance.lower() == 'yes':
        booksDict.clear()
        print("(·•᷄∩•᷅ ) Aww, all the books are now gone and deleted.")
    elif assurance.lower() == 'no':
        print("⸜(｡˃ ᵕ ˂ )⸝♡ Yehey! I knew you still loved the books! They were not deleted.")
    else:
        print("(·•᷄∩•᷅ ) Oh sorry, but that's an invalid option.")

def viewBook():
    # Ask user for the Title of the book they want to view
    title = input("⸜(｡˃ ᵕ ˂ )⸝♡ kitty! Can you input the title of the book you wanna see: ")

    bookFound = False  # Check if the book exists

    # Find the book
    for bookInfo in booksDict.values():
        if bookInfo['Title'] == title:
            bookFound = True
            # If the book has been found, print all information related to the book entry
            print("(..◜ᴗ◝..) Here's the book's information:")
            for key, value in bookInfo.items():
                print(f"{key}: {value}")
            
          # Print the names of the borrowers from borrow list Dictionary
            for borrowerID in bookInfo['List of Borrowers']:
                borrowInfo = borrowListDict.get(borrowerID)
                if borrowInfo:
                    logID = borrowInfo['Log ID']
                    logInfo = logbookDict.get(logID)
                    if logInfo:
                        borrowerName = logInfo.get('Person Name')
                        if borrowerName:
                            print(f"Borrower: {borrowerName}")
                    
    # If the book isn't found, print a message
    if not bookFound:
        print("૮₍˶ ╥ ‸ ╥ ⑅₎ა Sorry, we tried our best, but we can't find that book.")
        
#Function to edit the info of a book
def editBook():
    #Ask the user for the Title of the book they want to edit
    title = input("(..◜ᴗ◝..) Yow kitty! Can you input the title of the book you want to edit: ")

    # Find the book by title 
    for bookID, bookInfo in booksDict.items():
        if bookInfo['Title'] == title:
            # If book is found, let user enter a new Title, new Author, and new Date Published for the Book.
            print("(..◜ᴗ◝..) Hey, umm, can you input the new info for the book:")
            newTitle = input("New Title: ")
            newAuthor = input("New Author: ")
            newDatePublished = input("New Date Published (e.g. 20 Dec 2023): ")

            # Update book information
            bookInfo['Title'] = newTitle
            bookInfo['Author'] = newAuthor
            bookInfo['Date Published'] = newDatePublished
            print("٩(^ᗜ^ )و ´- Hooray! The book's information has been edited.")
            break
     #If book has not been found, print a message.    
    else:
        print("૮₍•᷄  ༝ •᷅₎ა Huhuhu, we really can't find that book.")

#Function to view pending books
def viewPending():
    #Check to see if there are no unavailable books
    if not any(book['Status'] == 'Unavailable' for book in booksDict.values()):
        print("૮₍•᷄  ༝ •᷅₎ა Oh, umm, there aren't any pending books yet.")
        return

    # Print all entries of the Books Dictionary that have the status “Unavailable”
    print("◝(ᵔᵕᵔ)◜ Here's the list of pending books:")
    for bookInfo in booksDict.values():
        if bookInfo['Status'] == 'Unavailable':
            print("Title:", bookInfo['Title'])
            print("Author:", bookInfo['Author'])
            print("Date Published:", bookInfo['Date Published'])
            print("Status:", bookInfo['Status'])
            
            # See if there are borrowers
            if bookInfo['List of Borrowers']:
                borrowerIDs = bookInfo['List of Borrowers']
                lastBorrowerInfo = None
                for borrowerID in reversed(borrowerIDs):
                    borrowerInfo = borrowListDict.get(borrowerID)
                    if borrowerInfo:
                        if borrowerInfo.get('Date Return'):
                            lastBorrowerInfo = borrowerInfo
                            break
                
                if lastBorrowerInfo:
                    lastBorrower = getBorrowerInfo(lastBorrowerInfo['Borrow ID'])
                    borrowerName = lastBorrower.get('Borrower')
                    dateReturn = lastBorrowerInfo.get('Date Return')
                    print("Last Borrower:", borrowerName)
                    print("Expected Date of Return:", dateReturn)
        
#Logbook dictionary and borrow list dictionary
logbookDict = {}
borrowListDict = {}

#Function to get borrower info
def getBorrowerInfo(borrowerID):
    
    #Borrower info dictionary
    borrowerInfo = {}
    
    #Find borrower
    for entry in borrowListDict.values():
        if entry['Borrow ID'] == borrowerID:
            #If borrower is found, update the dictionary
            borrowerInfo = {
            'Borrow ID': entry['Borrow ID'],
            'Book ID': entry['Book ID'],
            'Log ID': entry['Log ID'],
            'Date Return': entry['Date Return'],
            'Borrower': logbookDict[entry['Log ID']]['Person Name']
        }
        break
    return borrowerInfo
    
#Function to borrow a book
def borrowBook():
    #Create entry in Logbook Dictionary and have user provide information for the Log Entry(Person Name, Date, Time, Purpose)
    personName = input("(๑´>᎑<)~* Hey kitty! Can I know your name: ")
    date = input("(๑´>᎑<)~* Hehe, can you also input the date today (e.g. 20 Dec 2023): ")
    time = input("(๑´>᎑<)~* And lastly, also input the time now (e.g. 11:00 AM): ")
    purpose = "borrow" #Assuming the user will borrow a book

    # Ask user to provide the details of the Book they wish to borrow(Title, Author)
    bookTitle = input("₍ᵔ•ᴗ•ᵔ₎ Hey kitty! Can I know what's the title of the book you want to borrow: ")
    bookAuthor = input("₍ᵔ•ᴗ•ᵔ₎ Also, can I know its author: ")

    # Find the book
    bookToBorrow = None
    for bookID, bookInfo in booksDict.items():
        if bookInfo['Title'] == bookTitle and bookInfo['Author'] == bookAuthor:
            #If book is found, update the value of the variable
            bookToBorrow = bookInfo
            break

    if bookToBorrow:
        # Create entry in logbook dictionary
        logID = createLogID()
        logbookDict[logID] = {
            'Log ID': logID,
            'Person Name': personName,
            'Date': date,
            'Time': time,
            'Purpose': purpose
        }

        # Create entry in borrow list dictionary
        borrowID = createBorrowID()
        borrowListDict[borrowID] = {
            'Borrow ID': borrowID,
            'Log ID': logID,
            'Book ID': bookToBorrow['Book ID'],
            'Date Return': input("(,,>﹏<,,) Hey kitty, when do you plan on returning this book (e.g. 20 Dec 2023): ")
        }

        # Update book status to "Unavailable"
        bookToBorrow['Status'] = 'Unavailable'
        #Append Borrow_ID to the List of Borrowers in the Books Dictionary
        bookToBorrow['List of Borrowers'].append(borrowID)
        print(f"(∩˃o˂∩)♡ Yahoo! You have borrowed the book '{bookTitle}' by {bookAuthor} with the borrow ID '{borrowID}'.")
    else:
        print("<( °^° )> That's sad, we can't find the book you're looking for.")

#Funciton to create log ID
def createLogID():
    logCount = len(logbookDict) + 1
    logID = f"L{logCount}"
    return logID

#Global counter to check number of borrowers
borrowCounter = 0

#Function to create borrow ID
def createBorrowID():
    global borrowCounter
    borrowCounter += 1
    borrowID = f"BL{borrowCounter}"
    return borrowID

#Function to return a book
def returnBook():
    #Ask user for information
    personName = input("₍^⸝⸝> ·̫ <⸝⸝ ^₎ Hey kitty! Mind if I ask who's this: ")
    date = input("₍^⸝⸝> ·̫ <⸝⸝ ^₎ Can you also input the date today (e.g. 23 Dec 2023): ")
    time = input("₍^⸝⸝> ·̫ <⸝⸝ ^₎ Hehe, can you also input the time now (e.g. 11:00 AM): ")
    purpose = "return"  # Assuming the user will return a book

    borrowID = input("ʕ •ᴥ•ʔ Hey kitty! Can you input the borrow ID of the book you want to return: ")

    # Find the book to return
    if borrowID in borrowListDict:
        bookID = borrowListDict[borrowID]['Book ID']
        bookToReturn = booksDict[bookID]

        # Update book status to "Available"
        bookToReturn['Status'] = 'Available'

        # Create entry in logbook dictionary
        logID = createLogID()
        logbookDict[logID] = {
            'Log ID': logID,
            'Person Name': personName,
            'Date': date,
            'Time': time,
            'Purpose': purpose
        }
        # Remove book from borrowListDict
        del borrowListDict[borrowID]
        
        print(f"ʕ •ᴥ•ʔ Thank you very much for returning the  book with borrow ID '{borrowID}'")
    #If borrow ID is not found.
    else:
        print("(っ- ‸ - ς) Huhu sorry, but we can't find the book with that borrow ID.")

#Function to view all borrowers
def viewAllEntries():
    # Check if borrowList dictionary is empty
    if not borrowListDict:
        print("(っ- ‸ - ς) Oh, there seems to be no borrowed books at this time.")
        return
    #Print all Borrow_List Dictionary Entries
    print("⸜(｡˃ ᵕ ˂ )⸝♡ Here's the list of all borrowed books:")
    for borrowID, borrowInfo in borrowListDict.items():
        bookInfo = booksDict.get(borrowInfo['Book ID'])
        logInfo = logbookDict.get(borrowInfo['Log ID'])
        #Printed information
        if bookInfo and logInfo:
            print("Borrow ID:", borrowID)
            print("Title:", bookInfo['Title'])
            print("Author:", bookInfo['Author'])
            print("Date Published:", bookInfo['Date Published'])
            print("Date Return:", borrowInfo['Date Return'])
            print("Borrower:", logInfo['Person Name'])
            print("----------")

#Function to view expected returns in a specific day
def viewExpectedReturns():
    # Ask user for date to print
    date = input("(っ'ヮ'c) Hey kitty! Can you input the date that you want to see the expected returns (e.g. 20 Dec 2023): ")
    # Check if there are expected returns for the given date
    expectedReturns = False

    # Print all Borrow List Entries that match the Date Return stated
    print(f"(っ'ヮ'c) Here you go, the expected returns on {date}:")
    for borrowID, borrowInfo in borrowListDict.items():
        if borrowInfo['Date Return'] == date:
            bookInfo = booksDict.get(borrowInfo['Book ID'])
            logInfo = logbookDict.get(borrowInfo['Log ID'])
            
            # Printed information if bookInfo and logInfo exist
            if bookInfo and logInfo:
                print("Borrow ID:", borrowID)
                print("Title:", bookInfo['Title'])
                print("Author:", bookInfo['Author'])
                print("Date Published:", bookInfo['Date Published'])
                print("Status:", bookInfo['Status'])
                print("Borrower:", logInfo['Person Name'])
                print("----------")
                expectedReturns = True
                
    # If there are no expected returns, print a message
    if not expectedReturns:
        print(f"૮₍•᷄  ༝ •᷅₎ა Oh, it looks like we don't have any expected returns for {date}.")

#Function to visit library
def visitLibrary():
    #Ask user for the information to include in Log Entry(Person Name, Date, Time, Purpose)
    personName = input("(๑ᵔ⤙ᵔ๑) Hi kitty! Can we please know your name: ")
    date = input("(๑ᵔ⤙ᵔ๑) Also, can you input the date today (e.g. 9 Jan 2020): ")
    time = input("(๑ᵔ⤙ᵔ๑) Lastly, can you input the time now (e.g. 9:00 AM): ")
    purpose = "visit"  # Assuming the user will visit the library

    # Create entry in Logbook Dictionary
    logID = createLogID()
    logbookDict[logID] = {
        'Log ID': logID,
        'Person Name': personName,
        'Date': date,
        'Time': time,
        'Purpose': purpose
    }
    print(f".o(≧∀≦)o Yehey! We have a visitor in the library by the name of {personName}.")

#Function to view all who visited the library
def viewAllEntriesLogbook():
    #Check if there are any entries on the logbook
    if not logbookDict:
    	print(f"(っ- ‸ - ς) That's sad, looks like no one has logged in yet.")
    
    else:
    	print("ᐠ( ᐛ )ᐟ Here are all the entries in the logbook:")
    	for logID, logInfo in logbookDict.items():
        	print("Log ID:", logInfo['Log ID'])
        	print("Person Name:", logInfo['Person Name'])
        	print("Date:", logInfo['Date'])
        	print("Time:", logInfo['Time'])
        	print("Purpose:", logInfo['Purpose'])
        	print("----------")

#Function to view all transactions in a specific date
def viewTransactionsPerDay():
    # Ask user for date to print
    date = input("ᐠ( ᐛ )ᐟ Wassup kitty! Can you input which date do you want to see the transactions for that day (e.g. 20 Dec 2023): ")

    # Check if there are transactions for the given date
    transactions = False

    # Print all Log Book Entries that match the date stated
    print(f"ᐠ( ᐛ )ᐟ Here are all of the transactions on {date}:")
    for logID, logInfo in logbookDict.items():
        if logInfo['Date'] == date:
            print("Log ID:", logInfo['Log ID'])
            print("Person Name:", logInfo['Person Name'])
            print("Date:", logInfo['Date'])
            print("Time:", logInfo['Time'])
            print("Purpose:", logInfo['Purpose'])
            print("----------")
            transactions = True

    # If there are no transactions, print a message
    if not transactions:
        print(f"(っ- ‸ - ς) Oh, umm, we can't find transactions for {date}.")

# Use files to save state of Books Dictionary, Borrow_List Dictionary, and the Logbook Dictionary.
booksData = 'books.dat'
borrowListData = 'borrowList.dat'
logbookData = 'logbook.dat'
            
# Load dictionaries from files
def loadData():
        #Loading data from booksData
        with open(booksData, 'r') as file:
            data = file.read()
            if data:
                global booksDict
                booksDict = eval(data)

        #Loading data from borrowListData
        with open(borrowListData, 'r') as file:
            data = file.read()
            if data:
                global borrowListDict
                borrowListDict = eval(data)

        #Loading data from logbookData
        with open(logbookData, 'r') as file:
            data = file.read()
            if data:
                global logbookDict
                logbookDict = eval(data)

# Save dictionaries to files
def saveData():
    #Saving data to booksData
    with open(booksData, 'w') as file:
        file.write(str(booksDict))

    #Saving data to borrowListData
    with open(borrowListData, 'w') as file:
        file.write(str(borrowListDict))

    #Saving data to logbookData
    with open(logbookData, 'w') as file:
        file.write(str(logbookDict))
        
#Function for main menu
def mainMenu():
    print("≽^•.•^≼ Welcome to Kitty Library!  ฅ^•ﻌ•^ฅ")
    print("------------------------------------------")
    print("*** Main Menu *** ₍^ >ヮ<^₎")
    print("1. Books Section (  ≧ᗜ≦)")
    print("2. Borrowers Section (∩˃o˂∩)♡")
    print("3. Logbook Section ヽ(°〇°)ﾉ")
    print("4. Save Data ദ്ദി ˉ͈̀-.-ˉ͈́ )✧")
    print("5. Load Data (♡ˊ͈-,- ˋ͈)")
    print("6. Exit ૮(˶╥︿╥)ა")
    print("*** Main Menu *** ₍^ >ヮ<^₎")
    print("------------------------------------------")
    
#Function for books section menu
def booksSectionMenu():
    print("----------------------------------")
    print("*** Books Section Menu *** ₍^.  ̫.^₎")
    print("1. Add Book =^◕O◕^=")
    print("2. Delete Book (｡•́︿•̀｡)")
    print("3. Delete All Books (ᴗ_ ᴗ。)")
    print("4. View Book (°〇°)")
    print("5. Edit Book ( •͈૦•͈ )")
    print("6. View Pending (っ º - º ς)")
    print("7. Back to Main Menu (* °ヮ° *)")
    print("*** Books Section Menu *** ₍^.  ̫.^₎")
    print("----------------------------------")

#Function for borrowers section menu
def borrowersSectionMenu():
    print("--------------------------------------")
    print("*** Borrowers Section Menu *** ≽ܫ≼")
    print("1. Borrow Book ദ്ദി ˉ͈̀>.<ˉ͈́ )✧")
    print("2. Return Book ⸜(｡˃ ᵕ ˂ )⸝♡")
    print("3. View All Entries (ෆ˙ᵕ˙ෆ)♡")
    print("4. View Expected Returns (∩˃o˂∩)♡")
    print("5. Back to Main Menu (*´▽`*)")
    print("*** Borrowers Section Menu *** ≽ܫ≼")
    print("--------------------------------------")

#Function for logboook section menu
def logbookSectionMenu():
    print("------------------------------------------")
    print("*** Logbook Section Menu *** ໒´ ˘ ` ა")
    print("1. Visit Library ৻(  •̀ ᗜ •́  ৻)")
    print("2. View All Entries (✪▽✪)")
    print("3. View Transactions per Day o( ˶^▾^˶ )o")
    print("4. Back to Main Menu (♡ˊ͈ >.< ˋ͈)")
    print("*** Logbook Section Menu *** ໒´ ˘ ` ა")
    print("------------------------------------------")

# Choosing in main menu
while True:
    mainMenu()
    choice = input("૮₍ ˶ᵔ ᵕ ᵔ˶ ₎ა Hi kitty! Please select an option: ")

#Choosing 1 will bring you to the books section menu
    if choice == '1':
        while True:
            booksSectionMenu()
            booksChoice = input("૮₍ ˶ᵔ ᵕ ᵔ˶ ₎ა Hi kitty! Please select an option: ")
            
            #Choosing a number will bring you to a specific function 
            if booksChoice == '1':
                addBook()
            elif booksChoice == '2':
                deleteBook()
            elif booksChoice == '3':
                deleteAllBooks()
            elif booksChoice == '4':
                viewBook()
            elif booksChoice == '5':
                editBook()
            elif booksChoice == '6':
                viewPending()
            elif booksChoice == '7':
                break
            #Inputting an invalid choice would print a message
            else:
                print("｡°(°.◜O◝°)°｡ No, that's not right, please choose again.")

#Choosing 2 will bring you to the borrowers section menu
    elif choice == '2':
        while True:
            borrowersSectionMenu()
            borrowersChoice = input("૮₍ ˶ᵔ ᵕ ᵔ˶ ₎ა Hi kitty! Please select an option: ")
            
            #Choosing a number will bring you to a specific function
            if borrowersChoice == '1':
                borrowBook()
            elif borrowersChoice == '2':
                returnBook()
            elif borrowersChoice == '3':
                viewAllEntries()
            elif borrowersChoice == '4':
                viewExpectedReturns()
            elif borrowersChoice == '5':
                break
            #Inputting an invalid choice would print a message
            else:
                print("｡°(°.◜O◝°)°｡ No, that's not right, please choose again.")

#Choosing 3 will bring you to the logbook section menu
    elif choice == '3':
        while True:
            logbookSectionMenu()
            logbookChoice = input("૮₍ ˶ᵔ ᵕ ᵔ˶ ₎ა Hi kitty! Please select an option: ")
            
            if logbookChoice == '1':
                visitLibrary()
            elif logbookChoice == '2':
                viewAllEntriesLogbook()
            elif logbookChoice == '3':
                viewTransactionsPerDay()
            elif logbookChoice == '4':
                break
            #Inputting an invalid choice would print a message
            else:
                print("｡°(°.◜O◝°)°｡ No, that's not right, please choose again.")

#Choosing 4 will save your current data
    elif choice == '4':
        saveData()
        print(" =^◕O◕^= Yehey! Your current data has been saved.")
        
#Choosing 5 will load your previous data
    elif choice == '5':
        loadData()
        print(" =^◕O◕^= Alright! Your previous data has been loaded.")

#Choosing 6 will exit the program
    elif choice == '6':
        print(" (ㅠ﹏ㅠ) Aww, why you gotta leave so soon, we will miss you, bye bye.")
        exit()
#If invalid choice
    else:
        print("｡°(°.◜O◝°)°｡ No, that's not right, please choose again.")

mainMenu()
