//
//  main.cpp
//  AssemblyProject1
//
//  Created by Menna Wagdy on 24/11/2021.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <iomanip>
#include <sstream>
using namespace std;
void RType(string line, string InstructionName);
void UType(string line, string instructionName);
void addF(string rd, string rs1, string rs2);
void subF(string rd, string rs1, string rs2);
void sllF(string rd, string rs1, string rs2);
void slliF(string rd, string rs1, string imm);
void sltF(string rd, string rs1, string rs2);
void sltuF(string rd, string rs1, string rs2);
void xorF(string rd, string rs1, string rs2);
void srlF(string rd, string rs1, string rs2);
void srliF(string rd, string rs1, string imm);
void sraF(string rd, string rs1, string rs2);
void sraiF(string rd, string rs1, string imm);
void orF(string rd, string rs1, string rs2);
void andF(string rd, string rs1, string rs2);
void xoriI(string rs1,string rd,string imm);
void oriI(string rs1,string rd,string imm);
void andiI(string rs1,string rd,string imm);
void sltiI(string rs1,string rd,string imm);
void sltiuI(string rs1,string rd,string imm);
void addiI(string rs1,string rd,string imm);
void luiU( string rd, string imm);
void auipcU(string rd, string imm);
string decimalToBinary(int decimal);
int binaryToDecimal(string binary);
string decimalToHex(int decimal);
int findPosition(string xPosition);
unsigned int ProgramCounterValue = 0;
int flag = 0;
vector <string> code;
vector <string> Branches;
vector <int> LinesOfBranches;
int registerValueDecimal[32]; //This array contains the values of the registers
string registerValueBinary[32];
string registerValueHexadecimal[32];
vector<vector <int>> Memory; //The first column contains the memory address while the second value contains the value.
vector<string> MemoryBinary;
vector<string> MemoryHexadecimal;
 
//Helper Functions
string decimalToBinary(int decimal)
{
    int binary[32];
    string binaryString = "";
    int i = 0;
    while (decimal > 0) {
        binary[i] = decimal % 2;
        decimal = decimal / 2;
        binaryString += to_string(binary[i]);
        i++;
    }
    for (int i = 0; i < binaryString.length() / 2; i++) {
        swap(binaryString[i], binaryString[binaryString.length() - i - 1]);
    }
    return binaryString;
}
 
int binaryToDecimal(string binary) //32 bits
{
    int x = binary.find('1');
    string shorter = binary.substr(x, binary.length() - x);
    int decimal = 0;
    int base = 1;
    int temp = stoi(shorter);
    while (temp) {
        int lastDigit = temp % 10;
        temp = temp / 10;
        decimal = decimal + lastDigit * base;
        base = base * 2;
    }
    return decimal;
}
 
int findPosition(string xPosition)
{
    int position;
    position = stoi(xPosition.substr(1, xPosition.length() - 1));
    return position;
}
 
int searchBranch(vector<string> Branches, string branchName)
{
    for (size_t i = 0; i < Branches.size(); i++)
    {
        if (branchName == Branches[i])
            return i;
    }
    return -1; //Branch not found
}
 
int findMemoryAddress(vector<vector <int>> Memory, int MemoryAddress)
{
    for (int i = 0; i < Memory.size(); i++)
    {
        if (Memory[i][0] == MemoryAddress)
            return i;
    }
    return -1;
}
//End of Helper Functions
 
void jalI(int rd, string branchName)
{
    int index;
    index = searchBranch(Branches, branchName);
    registerValueDecimal[rd] = ProgramCounterValue+1;
    registerValueBinary[rd] = decimalToBinary(ProgramCounterValue+1);
    //registerValueHexadecimal[rd]= decimalToHexadecimal(ProgramCounterValue+1);
    registerValueHexadecimal[rd] = "Testing";
    
    ProgramCounterValue = LinesOfBranches[index];
}
 
//Branch Functions
void beqI(int rs1, int rs2, string branchName)
{
    int index, goToLine;
    index = searchBranch(Branches, branchName);
 
    if (rs1 == rs2)
    {
        goToLine = LinesOfBranches[index];
        ProgramCounterValue = goToLine;
    }
    else
        ProgramCounterValue = ProgramCounterValue+4;
}
void bneI(int rs1, int rs2, string branchName)
{
    int index, goToLine;
    index = searchBranch(Branches, branchName);
 
    if (rs1 != rs2)
    {
        goToLine = LinesOfBranches[index];
        ProgramCounterValue = goToLine;
    }
    else
        ProgramCounterValue = ProgramCounterValue+4;
}
void bltI(int rs1, int rs2, string branchName)
{
    int index, goToLine;
    index = searchBranch(Branches, branchName);
 
    if (rs1 < rs2)
    {
        goToLine = LinesOfBranches[index];
        ProgramCounterValue = goToLine;
    }
    else
        ProgramCounterValue = ProgramCounterValue+4;
}
void bgeI(int rs1, int rs2, string branchName)
{
    int index, goToLine;
    index = searchBranch(Branches, branchName);
 
    if (rs1 >= rs2)
    {
        goToLine = LinesOfBranches[index];
        ProgramCounterValue = goToLine;
    }
    else
        ProgramCounterValue = ProgramCounterValue+4;
}
void bltuI(int rs1, int rs2, string branchName)
{
    int index, goToLine;
    index = searchBranch(Branches, branchName);
 
    unsigned int rs1Unsigned = (unsigned int)rs1;
    unsigned int rs2Unsigned = (unsigned int)rs2;
 
    if (rs1Unsigned < rs2Unsigned)
    {
        goToLine = LinesOfBranches[index];
        ProgramCounterValue = goToLine;
    }
    else
        ProgramCounterValue = ProgramCounterValue+4;
}
void bgeuI(int rs1, int rs2, string branchName)
{
    int index, goToLine;
    index = searchBranch(Branches, branchName);
 
    unsigned int rs1Unsigned = (unsigned int)rs1;
    unsigned int rs2Unsigned = (unsigned int)rs2;
 
    if (rs1Unsigned >= rs2Unsigned)
    {
        goToLine = LinesOfBranches[index];
        ProgramCounterValue = goToLine;
    }
    else
        ProgramCounterValue = ProgramCounterValue+4;
}
 
//Jalr
void jalrI(int rd, int offset, int rs1)
{
   int goToAddress = registerValueDecimal[rs1]+offset;
    
    registerValueDecimal[rd] = ProgramCounterValue+1;
    registerValueBinary[rd] = decimalToBinary(ProgramCounterValue+1);
    //registerValueHexadecimal[rd]= decimalToHexadecimal(ProgramCounterValue+1);
    registerValueHexadecimal[rd] = "Testing";
    
    ProgramCounterValue = goToAddress;
}
 
//Load Functions
void lbI(int destination, int offset, int base)
{
    int memoryAddress = base + offset;
    int row = findMemoryAddress(Memory, memoryAddress);
    string byte = MemoryBinary[row];
 
    registerValueDecimal[destination] = binaryToDecimal(byte);
    registerValueBinary[destination] = byte;
    //registerValueHexadecimal[destination]= binaryToHexadecimal(byte);
    registerValueHexadecimal[destination] = "Testing";
}
 
void lhI(int destination, int offset, int base)
{
    int memoryAddress = base + offset;
    
    int row1 = findMemoryAddress(Memory, memoryAddress);
    string halfBaseValue8 = MemoryBinary[row1];
    int row2 = findMemoryAddress(Memory, memoryAddress+1);
    string halfBaseValue0 = MemoryBinary[row2];
    
    string halfWord = halfBaseValue0+halfBaseValue8;
    if (halfWord[0]=='1')
        halfWord = "1111111111111111" + halfWord;
    else if (halfWord[0]=='0')
        halfWord = "0000000000000000" + halfWord;
    
    registerValueDecimal[destination] = binaryToDecimal(halfWord);
    registerValueBinary[destination] = halfWord;
    //registerValueHexadecimal[destination]= binaryToHexadecimal(halfWord);
    registerValueHexadecimal[destination] = "Testing";
}
 
void lwI(int destination, int offset, int base)
{
    int memoryAddress = base + offset;
    int row1 = findMemoryAddress(Memory, memoryAddress);
    string BaseValue24 = MemoryBinary[row1];
    int row2 =findMemoryAddress(Memory, memoryAddress+1);
    string BaseValue16 = MemoryBinary[row2];
    int row3 =findMemoryAddress(Memory, memoryAddress+2);
    string BaseValue8 = MemoryBinary[row3];
    int row4 =findMemoryAddress(Memory, memoryAddress+3);
    string BaseValue0 = MemoryBinary[row4];
    
    string result=BaseValue0+BaseValue8+BaseValue16+BaseValue24;
 
    registerValueDecimal[destination] = binaryToDecimal(result);
    registerValueBinary[destination] = result;
    //registerValueHexadecimal[destination]= binaryToHexadecimal(result);
    registerValueHexadecimal[destination] = "Testing";
}
 
void lbuI(int destination, int offset, int base)
{
    int memoryAddress = base + offset;
    int row = findMemoryAddress(Memory, memoryAddress);
    string byte = MemoryBinary[row];
    byte = "000000000000000000000000" + byte;
 
    registerValueDecimal[destination] = binaryToDecimal(byte);
    registerValueBinary[destination] = byte;
    //registerValueHexadecimal[destination]= binaryToHexadecimal(byte);
    registerValueHexadecimal[destination] = "Testing";
}
 
void lhuI(int destination, int offset, int base)
{
    int memoryAddress = base + offset;
    
    int row1 = findMemoryAddress(Memory, memoryAddress);
    string halfBaseValue8 = MemoryBinary[row1];
    int row2 = findMemoryAddress(Memory, memoryAddress+1);
    string halfBaseValue0 = MemoryBinary[row2];
    
    string halfWord = halfBaseValue0+halfBaseValue8;
    halfWord = "0000000000000000" + halfWord;
    
    registerValueDecimal[destination] = binaryToDecimal(halfWord);
    registerValueBinary[destination] = halfWord;
    //registerValueHexadecimal[destination]= binaryToHexadecimal(halfWord);
    registerValueHexadecimal[destination] = "Testing";
}
 
//Store Functions
void sbI(string sourceValue, int offset, int baseValue)
{
    int destinationMemoryAddress = baseValue + offset;
    string byte = sourceValue.substr(24, 8); //Takes the least significant 8 bits of the string
 
    int ifAlreadyPresent = findMemoryAddress(Memory, destinationMemoryAddress);
    if (ifAlreadyPresent != -1)
    {
        Memory[ifAlreadyPresent][1] = binaryToDecimal(byte);
        MemoryBinary[ifAlreadyPresent] = byte;
        //MemoryHexadecimal[ifAlreadyPresent] = binaryToHexadecimal(byte);
        MemoryHexadecimal[ifAlreadyPresent] = "Testing";
    }
    else
    {
        vector<int> row{ destinationMemoryAddress, binaryToDecimal(byte) };
        Memory.push_back(row);
        MemoryBinary.push_back(byte);
        //MemoryHexadecimal.push_back(binaryToHexadecimal(byte));
        MemoryHexadecimal.push_back("Testing");
    }
}
void shI(string sourceValue, int offset, int baseValue)
{
    int destinationMemoryAddress = baseValue + offset;
    string halfSource = sourceValue.substr(16, 16); //Takes the least significant 16 bits of the string
    string halfSource8 = halfSource.substr(8, 8);
    string halfSource0 = halfSource.substr(0, 8);
 
    int ifAlreadyPresent = findMemoryAddress(Memory, destinationMemoryAddress);
    if (ifAlreadyPresent != -1)
    {
        Memory[ifAlreadyPresent][1] = binaryToDecimal(halfSource8);
        MemoryBinary[ifAlreadyPresent] = halfSource8;
        //MemoryHexadecimal[ifAlreadyPresent] = binaryToHexadecimal(halfSource8);
        MemoryHexadecimal[ifAlreadyPresent] = "Testing";
    }
    else
    {
        vector<int> row{ destinationMemoryAddress, binaryToDecimal(halfSource8) };
        Memory.push_back(row);
        MemoryBinary.push_back(halfSource8);
        //MemoryHexadecimal.push_back(binaryToHexadecimal(halfSource8));
        MemoryHexadecimal.push_back("Testing");
    }
 
    ifAlreadyPresent = findMemoryAddress(Memory, destinationMemoryAddress + 1);
    if (ifAlreadyPresent != -1)
    {
        Memory[ifAlreadyPresent + 1][1] = binaryToDecimal(halfSource0);
        MemoryBinary[ifAlreadyPresent] = halfSource0;
        //MemoryHexadecimal[ifAlreadyPresent + 1] = binaryToHexadecimal(halfSource0);
        MemoryHexadecimal[ifAlreadyPresent+1] = "Testing";
    }
    else
    {
        vector<int> row{ destinationMemoryAddress + 1, binaryToDecimal(halfSource0) };
        Memory.push_back(row);
        MemoryBinary.push_back(halfSource0);
        //MemoryHexadecimal.push_back(binaryToHexadecimal(halfSource0));
        MemoryHexadecimal.push_back("Testing");
    }
}
void swI(string sourceValue, int offset, int baseValue)
{
    int destinationMemoryAddress = baseValue + offset;
 
    string byte0, byte8, byte16, byte24;
    byte0 = sourceValue.substr(0, 8);
    byte8 = sourceValue.substr(8, 8);
    byte16 = sourceValue.substr(16, 8);
    byte24 = sourceValue.substr(24, 8);
 
    int ifAlreadyPresent = findMemoryAddress(Memory, destinationMemoryAddress);
    if (ifAlreadyPresent != -1)
    {
        Memory[ifAlreadyPresent][1] = binaryToDecimal(byte24);
        MemoryBinary[ifAlreadyPresent] = byte24;
        //MemoryHexadecimal[ifAlreadyPresent] = binaryToHexadecimal(byte24);
        MemoryHexadecimal[ifAlreadyPresent] = "Testing";
    }
    else
    {
        vector<int> row{ destinationMemoryAddress, binaryToDecimal(byte24) };
        Memory.push_back(row);
        MemoryBinary.push_back(byte24);
        //MemoryHexadecimal.push_back(binaryToHexadecimal(byte24));
        MemoryHexadecimal.push_back("Testing");
    }
    ifAlreadyPresent = findMemoryAddress(Memory, destinationMemoryAddress + 1);
    if (ifAlreadyPresent != -1)
    {
        Memory[ifAlreadyPresent + 1][1] = binaryToDecimal(byte16);
        MemoryBinary[ifAlreadyPresent] = byte16;
        //MemoryHexadecimal[ifAlreadyPresent + 1] = binaryToHexadecimal(byte16);
        MemoryHexadecimal[ifAlreadyPresent+1] = "Testing";
    }
    else
    {
        vector<int> row{ destinationMemoryAddress + 1, binaryToDecimal(byte16) };
        Memory.push_back(row);
        MemoryBinary.push_back(byte16);
        //MemoryHexadecimal.push_back(binaryToHexadecimal(byte16));
        MemoryHexadecimal.push_back("Testing");
    }
    ifAlreadyPresent = findMemoryAddress(Memory, destinationMemoryAddress + 2);
    if (ifAlreadyPresent != -1)
    {
        Memory[ifAlreadyPresent + 2][1] = binaryToDecimal(byte8);
        MemoryBinary[ifAlreadyPresent] = byte8;
        //MemoryHexadecimal[ifAlreadyPresent + 2] = binaryToHexadecimal(byte8);
        MemoryHexadecimal[ifAlreadyPresent+2] = "Testing";
    }
    else
    {
        vector<int> row{ destinationMemoryAddress + 2, binaryToDecimal(byte8) };
        Memory.push_back(row);
        MemoryBinary.push_back(byte8);
        //MemoryHexadecimal.push_back(binaryToHexadecimal(byte8));
        MemoryHexadecimal.push_back("Testing");
    }
    ifAlreadyPresent = findMemoryAddress(Memory, destinationMemoryAddress + 3);
    if (ifAlreadyPresent != -1)
    {
        Memory[ifAlreadyPresent + 3][1] = binaryToDecimal(byte0);
        MemoryBinary[ifAlreadyPresent] = byte0;
        //MemoryHexadecimal[ifAlreadyPresent + 3] = binaryToHexadecimal(byte0);
        MemoryHexadecimal[ifAlreadyPresent+3] = "Testing";
    }
    else
    {
        vector<int> row{ destinationMemoryAddress + 3, binaryToDecimal(byte0) };
        Memory.push_back(row);
        MemoryBinary.push_back(byte0);
        //MemoryHexadecimal.push_back(binaryToHexadecimal(byte0));
        MemoryHexadecimal.push_back("Testing");
    }
}
 
//Types of Instructions
void JType(string line)
{
    int commaPosition1 = line.find(',');
    string rd, branchName;
    rd = line.substr(4, commaPosition1 - 4);
    branchName = line.substr(commaPosition1+2, line.length() - (commaPosition1+2));
  
    int destination = findPosition(rd);
    jalI(destination, branchName);
}
 
void IType(string line, string instructionName)
{
    int commaPosition = line.find(',');
    int bracket1Position = line.find('(');
    int bracket2Position = line.find(')');
    string rd, offset, rs1;
    int destination = findPosition(rd);
    int offsetInteger = stoi(offset);
    int source = findPosition(rs1);
    
    if (instructionName=="jalr")
    {
        rd = line.substr(5, commaPosition - 5);
        offset = line.substr(commaPosition + 2, bracket1Position - (commaPosition + 2));
        rs1 = line.substr(bracket1Position + 1, bracket2Position - (bracket1Position + 1));
        jalrI(destination, offsetInteger, source);
    }
    else if ((instructionName == "lb")||(instructionName == "lh") ||(instructionName == "lw"))
    {
        ProgramCounterValue=ProgramCounterValue+4;
        rd = line.substr(3, commaPosition - 3);
        offset = line.substr(commaPosition + 2, bracket1Position - (commaPosition + 2));
        rs1= line.substr(bracket1Position + 1, bracket2Position - (bracket1Position + 1));
        
        if (instructionName == "lb")
            lbI(destination, offsetInteger, registerValueDecimal[source]);
        else if (instructionName=="lh")
            lhI(destination, offsetInteger, registerValueDecimal[source]);
        else if (instructionName=="lw")
            lwI(destination, offsetInteger, registerValueDecimal[source]);
    }
    else if((instructionName == "lbu") || (instructionName=="lhu"))
    {
        rd = line.substr(4, commaPosition - 4);
        offset = line.substr(commaPosition + 2, bracket1Position - (commaPosition + 2));
        rs1= line.substr(bracket1Position + 1, bracket2Position - (bracket1Position + 1));
        
        if (instructionName=="lbu")
            lbuI(destination, offsetInteger, registerValueDecimal[source]);
        else if (instructionName=="lhu")
            lhuI(destination, offsetInteger, registerValueDecimal[source]);
    }
}
 
void BType(string line, string instructionName)
{
    int commaPosition1 = line.find(',');
    string line2 = line.substr(commaPosition1 + 2, line.length() - (commaPosition1 + 2));
    int commaPosition2 = line2.find(',');
    string rs1, rs2, branchName;
 
    if ((instructionName == "beq") || (instructionName == "bne") || (instructionName == "blt") || (instructionName == "bge"))
    {
        rs1 = line.substr(4, commaPosition1 - 4);
        rs2 = line2.substr(0, commaPosition2);
        branchName = line2.substr(commaPosition2 + 2, line.length() - (commaPosition2 + 2));
    }
    else if ((instructionName == "bltu") || (instructionName == "bgeu"))
    {
        rs1 = line.substr(5, commaPosition1 - 5);
        rs2 = line2.substr(0, commaPosition2);
        branchName = line2.substr(commaPosition2 + 2, line.length() - (commaPosition2 + 2));
    }
 
    cout << "This is rs1: " << rs1 << endl;
    cout << "This is rs2: " << rs2 << endl;
    cout << "This is the branch name: " << branchName << endl;
 
    int register1 = findPosition(rs1);
    int register2 = findPosition(rs2);
 
    if (instructionName == "beq")
        beqI(registerValueDecimal[register1], registerValueDecimal[register2], branchName);
    if (instructionName == "bne")
        bneI(registerValueDecimal[register1], registerValueDecimal[register2], branchName);
    if (instructionName == "blt")
        bltI(registerValueDecimal[register1], registerValueDecimal[register2], branchName);
    if (instructionName == "bge")
        bgeI(registerValueDecimal[register1], registerValueDecimal[register2], branchName);
    if (instructionName == "bltu")
        bltuI(registerValueDecimal[register1], registerValueDecimal[register2], branchName);
    if (instructionName == "bgeu")
        bgeuI(registerValueDecimal[register1], registerValueDecimal[register2], branchName);
}
 
void SType(string line, string instructionName)
{
    ProgramCounterValue = ProgramCounterValue + 4;
    int commaPosition = line.find(',');
    string source = line.substr(3, commaPosition - 3);
 
    int bracket1Position = line.find('(');
    string offset = line.substr(commaPosition + 2, bracket1Position - (commaPosition + 2));
 
    int bracket2Position = line.find(')');
    string base = line.substr(bracket1Position + 1, bracket2Position - (bracket1Position + 1));
 
    cout << "This is the source: " << source << endl;
    cout << "This is the offset: " << offset << endl;
    cout << "This is the base: " << base << endl;
 
    int sourceInteger = findPosition(source);
    int offsetInteger = stoi(offset);
    int baseInteger = findPosition(base);
 
    if (instructionName == "sb")
        sbI(registerValueBinary[sourceInteger], offsetInteger, registerValueDecimal[baseInteger]);
    else if (instructionName == "sh")
        shI(registerValueBinary[sourceInteger], offsetInteger, registerValueDecimal[baseInteger]);
    else if (instructionName == "sw")
        swI(registerValueBinary[sourceInteger], offsetInteger, registerValueDecimal[baseInteger]);
}

void UType(string line, string instructionName)
{   string rd,imm;
    int commaPosition=line.find(',');  //finding the comma position in the line
    
    ProgramCounterValue=ProgramCounterValue+4;
    
     if (instructionName== "lui" )   //3 letters
    {
        rd=line.substr(4,commaPosition-4);
       
        imm= line.substr(commaPosition+2,line.length()-(commaPosition+2));
        
    }
         if (instructionName== "auipc" )   //5 letters
        {
            rd=line.substr(6,commaPosition-6);
           
            imm= line.substr(commaPosition+2,line.length()-(commaPosition+2));
            
        }
    cout<< "This is the destination register:" << rd << endl;
    cout<< "This is the immediate:" << imm << endl;
    
    if(instructionName=="lui")
    {
        luiU(rd,imm);
    }
    if (instructionName== "auipc")
    {
        auipcU(rd,imm);
    }
}

 
void TypeOfInstruction(string line)
{
    int spacePosition = line.find(" ");
    string x = line.substr(0, spacePosition); //Reading the first word in every line. x is the name of the instruction
 
//    if ((x == "lui") || (x == "auipc"))
//       UType(line, x);
    if (x == "jal") // I do not know which type is JAL
        JType(line);
    else if (x == "jalr")
        IType(line, x);
    else if ((x == "beq") || (x == "bne") || (x == "blt") || (x == "bge") || (x == "bltu") || (x == "bgeu"))
        BType(line, x);
    else if ((x == "lb") || (x == "lh") || (x == "lw") || (x == "lbu") || (x == "lhu"))
        IType(line, x);
    else if ((x == "sb") || (x == "sh") || (x == "sw"))
        SType(line, x);
    else if ((x == "addi") || (x == "slti") || (x == "sltiu") || (x == "xori") || (x == "ori") || (x == "andi"))
        IType(line, x);
    else if ((x == "slli") || (x == "srli") || (x == "srai") || (x == "add") || (x == "sub") || (x == "sll") || (x == "slt") || (x == "sltu") || (x == "xor") || (x == "srl") || (x == "sra") || (x == "or") || (x == "and"))
        RType(line, x);
    else if ((x == "fence") || (x == "ecall") || (x == "ebreak"))
        flag = 1; //This is a halting instruction, so it will end the execution of the program
    else //This means this is a branch. We already stored the branches and their respective line number in vectors, so we just increment the PCV.
        ProgramCounterValue=ProgramCounterValue+4;
}
 
void DisplayMemoryContents(vector<vector<int>> Memory, vector<string> MemoryBinary, vector<string> MemoryHexadecimal)
{
    cout << "Memory Address\t\tValue in Decimal\t\tValue in Binary\t\tValue in Hexadecimal\n";
    for (size_t i = 0; i < Memory.size(); i++)
    {
        cout << Memory[i][0] << "\t\t\t" << Memory[i][1] << "\t\t\t\t" << MemoryBinary[i] << "\t\t" << MemoryHexadecimal[i] << endl;
    }
}
 
void DisplayRegisterFileContents(int decimal[], string binary[], string hexadecimal[])
{
    for (int i = 0; i < 32; i++)
    {
        cout << "x" << i << "\t" << decimal[i] << "\t" << binary[i] << "\t" << hexadecimal[i] << endl;
    }
}
 
int main()
{
    string registerName[32][3];
    registerName[0][0]= "x0";
    registerName[0][1]= "zero";
    registerName[0][2]= stoi("00000");

    registerName[1][0]= "x1";
    registerName[1][1]= "ra";
    registerName[1][2]= stoi("00001");

    registerName[2][0]= "x2";
    registerName[2][1]= "sp";
    registerName[2][2]= stoi("00010");

    registerName[3][0]= "x3";
    registerName[3][1]= "gp";
    registerName[3][2]= stoi("00011");

    registerName[4][0]= "x4";
    registerName[4][1]= "tp";
    registerName[4][2]= stoi("00100");
    
    registerName[5][0]= "x5";
    registerName[5][1]= "t0";
    registerName[5][2]= stoi("00101");

    registerName[6][0]= "x6";
    registerName[6][1]= "t1";
    registerName[6][2]= stoi("00110");

    registerName[7][0]= "x7";
    registerName[7][1]= "t2";
    registerName[7][2]= stoi("00111");

    registerName[8][0]= "x8";
    registerName[8][1]= "s0";
    registerName[8][2]= stoi("01000");

    registerName[9][0]= "x9";
    registerName[9][1]= "s1";
    registerName[9][2]= stoi("01001");

    registerName[10][0]= "x10";
    registerName[10][1]= "a0";
    registerName[10][2]= stoi("01010");

    registerName[11][0]= "x11";
    registerName[11][1]= "a1";
    registerName[11][2]= stoi("01011");

    registerName[12][0]= "x12";
    registerName[12][1]= "a2";
    registerName[12][2]= stoi("01100");

    registerName[13][0]= "x13";
    registerName[13][1]= "a3";
    registerName[13][2]= stoi("01101");

    registerName[14][0]= "x14";
    registerName[14][1]= "a4";
    registerName[14][2]= stoi("01110");

    registerName[15][0]= "x15";
    registerName[15][1]= "a5";
    registerName[15][2]= stoi("01111");

    registerName[16][0]= "x16";
    registerName[16][1]= "a6";
    registerName[16][2]= stoi("10000");

    registerName[17][0]= "x17";
    registerName[17][1]= "a7";
    registerName[17][2]= stoi("10001");

    registerName[18][0]= "x18";
    registerName[18][1]= "s2";
    registerName[18][2]= stoi("10010");

    registerName[19][0]= "x19";
    registerName[19][1]= "s3";
    registerName[19][2]= stoi("10011");

    registerName[20][0]= "x20";
    registerName[20][1]= "s4";
    registerName[20][2]= stoi("10100");

    registerName[21][0]= "x21";
    registerName[21][1]= "s5";
    registerName[21][2]= stoi("10101");

    registerName[22][0]= "x22";
    registerName[22][1]= "s6";
    registerName[22][2]= stoi("10110");

    registerName[23][0]= "x23";
    registerName[23][1]= "s7";
    registerName[23][2]= stoi("10111");

    registerName[24][0]= "x24";
    registerName[24][1]= "s8";
    registerName[24][2]= stoi("11000");

    registerName[25][0]= "x25";
    registerName[25][1]= "s9";
    registerName[25][2]= stoi("11001");

    registerName[26][0]= "x26";
    registerName[26][1]= "s10";
    registerName[26][2]= stoi("11010");

    registerName[27][0]= "x27";
    registerName[27][1]= "s11";
    registerName[27][2]= stoi("11011");

    registerName[28][0]= "x28";
    registerName[28][1]= "t3";
    registerName[28][2]= stoi("11100");

    registerName[29][0]= "x29";
    registerName[29][1]= "t4";
    registerName[29][2]= stoi("11101");

    registerName[30][0]= "x30";
    registerName[30][1]= "t5";
    registerName[30][2]= stoi("11110");

    registerName[31][0]= "x31";
    registerName[31][0]= "t6";
    registerName[31][2]= stoi("11111");
    for (int i = 0; i < 32; i++) //Initializing the array
    {
        registerValueDecimal[i] = 0;
        registerValueBinary[i] = "00000000000000000000000000000000";
        registerValueHexadecimal[i] = "00000000";
    }
 
    //string InstructionsFileName = "i.txt";
    string InstructionsFileName = "/Users/mennawagdy/Desktop/Uni/Fall 2021/Assembly/AssemblyProject1/AssemblyProject1/i.txt";
    //string MemoryFileName = "m.txt";
    string MemoryFileName = "/Users/mennawagdy/Desktop/Uni/Fall 2021/Assembly/AssemblyProject1/AssemblyProject1/m.txt";
    string lineI, lineM;
    int lineNumber = -4;
 
    ifstream infileI;
    ifstream infileM;
    int memoryAddress, valueDecimal, valueBinary;
 
    infileM.open(MemoryFileName); //Checking that the file successfully opens
    if (!infileM.is_open())
    {
        cout << "The memory addresses file cannot be opened\n";
        return 0;
    }
 
    while (!infileM.eof()) //I read the text file and store the memory addresses and the values in a 2D vector
    {
        getline(infileM, lineM); //Reading each and every line in the file
        if(lineM.size() <= 0){
        int commaPosition1 = lineM.find(',');
        memoryAddress = stoi(lineM.substr(0, commaPosition1));
        valueDecimal = stoi(lineM.substr(commaPosition1 + 2, lineM.length() - (commaPosition1 + 2)));
        vector<int> row{ memoryAddress, valueDecimal };
        Memory.push_back(row);
        MemoryBinary.push_back(decimalToBinary(valueDecimal)); //Returns binary as an 8-bit integer
        //MemoryHexadecimal.push_back(decimalToHexadecimal(valueDecimal));
        MemoryHexadecimal.push_back("Testing");
        }
    }
 
    infileI.open(InstructionsFileName); //Checking that the file successfully opens
    if (!infileI.is_open())
    {
        cout << "The instructions file cannot be opened\n";
        return 0;
    }
 
    while (!infileI.eof()) //I read the text file and store every line in a vector of strings called code
    {
        getline(infileI, lineI); //Reading each and every line in the file
        lineNumber=lineNumber+4;
        code.push_back(lineI);
        int spacePosition = lineI.find(" ");
        string firstWord = lineI.substr(0, spacePosition); //Reading the first word in every line. firstWord is the name of the instruction
 
        if (firstWord.back() == ':')
        {
            firstWord.pop_back(); //Removing the last character (the colon) from the string
            Branches.push_back(firstWord);
            LinesOfBranches.push_back(lineNumber);
        }
    }
 
    for (int i = 0; i < Branches.size(); i++)
        cout << Branches[i] << "\t" << LinesOfBranches[i] << endl;
 
    while ((ProgramCounterValue < code.size()) && (flag == 0))
    {
        cout << "The program counter value is " << ProgramCounterValue << endl;
        DisplayRegisterFileContents(registerValueDecimal, registerValueBinary, registerValueHexadecimal);
        DisplayMemoryContents(Memory, MemoryBinary, MemoryHexadecimal);
        TypeOfInstruction(code[ProgramCounterValue]);
    }
 
    cout << "The program counter value is " << ProgramCounterValue << endl;
    DisplayRegisterFileContents(registerValueDecimal, registerValueBinary, registerValueHexadecimal);
    DisplayMemoryContents(Memory, MemoryBinary, MemoryHexadecimal);
 
    infileI.close();
    return 0;
}

void RType(string line, string InstructionName)
{
      string rs1, rs2, rd, imm;
    ProgramCounterValue=ProgramCounterValue+4;
    int commaPosition=line.find(',');
    string line2= line.substr(commaPosition+2, line.length()-(commaPosition+2));
    int commaPosition2= line2.find(',');
    if (InstructionName=="add") {
        rd= line.substr(4, commaPosition-4);
        rs1=line2.substr(0, commaPosition2);
        rs2=line2.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        addF(rd, rs1, rs2);
    }
    else if (InstructionName== "sub")
    {
        rd= line.substr(4, commaPosition-4);
        rs1=line2.substr(0, commaPosition2);
        rs2=line.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        subF(rd, rs1, rs2);
    }
    else if (InstructionName=="sll")
    {
        rd= line.substr(4, commaPosition-4);
        rs1=line2.substr(0, commaPosition2);
        rs2=line.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        sllF(rd, rs1, rs2);
    }
    else if (InstructionName=="slli")
    {
        rd= line.substr(4, commaPosition-4);
        rs1=line2.substr(0, commaPosition2);
        imm= line.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        slliF(rd, rs1, imm);
    }
    else if (InstructionName=="slt")
    {
        rd= line.substr(4, commaPosition-4);
        rs1=line2.substr(0, commaPosition2);
        rs2=line.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        sltF(rd, rs1, rs2);
    }
    else if(InstructionName=="sltu")
    {
        rd= line.substr(5, commaPosition-5);
        rs1=line2.substr(0, commaPosition2);
        rs2=line.substr(commaPosition2+2, line2.length());
        sltuF(rd, rs1, rs2);
    }
    else if (InstructionName=="xor")
    {
        rd= line.substr(4, commaPosition-4);
        rs1=line2.substr(0, commaPosition2);
        rs2=line.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        xorF(rd, rs1, rs2);
    }
    else if (InstructionName=="srl")
    {
        rd= line.substr(4, commaPosition-4);
        rs1=line2.substr(0, commaPosition2);
        rs2=line.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        srlF(rd, rs1, rs2);
    }
    else if (InstructionName=="srli")
    {
        rd= line.substr(4, commaPosition-4);
        rs1=line2.substr(0, commaPosition2);
        imm=line.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        srliF(rd, rs1, imm);
    }
    else if (InstructionName=="sra")
    {
        rd= line.substr(4, commaPosition-4);
        rs1=line2.substr(0, commaPosition2);
        rs2=line.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        sraF(rd, rs1, rs2);
    }
    else if (InstructionName=="srai")
    {
        rd= line.substr(4, commaPosition-4);
        rs1=line2.substr(0, commaPosition2);
        imm=line.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        sraiF(rd, rs1, imm);
    }
    else if (InstructionName=="or")
    {
        rd= line.substr(3, commaPosition-3);
        rs1=line2.substr(0, commaPosition2);
        rs2=line.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        orF(rd, rs1, rs2);
    }
    else if (InstructionName=="and")
    {
        rd= line.substr(4, commaPosition-4);
        rs1=line2.substr(0, commaPosition2);
        rs2=line.substr(commaPosition2+2, line2.length()-(commaPosition2+2));
        andF(rd, rs1, rs2);
    }
}


void addF(string rd, string rs1, string rs2)
{
    int rdv, rs1v, rs2v;
    string firstNumber, secondNumber, result="";
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    rs2v= findPosition(rs2);
    registerValueDecimal[rdv]=registerValueDecimal[rs1v]+registerValueDecimal[rs2v];
    registerValueBinary[rdv]=decimalToBinary(registerValueDecimal[rdv]);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
}

void subF(string rd, string rs1, string rs2)
{
    int rdv, rs1v, rs2v;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    rs2v= findPosition(rs2);
    registerValueDecimal[rdv]= registerValueDecimal[rs1v]-registerValueDecimal[rs2v];
    registerValueBinary[rdv]=decimalToBinary(registerValueDecimal[rdv]);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);

}
void sllF(string rd, string rs1, string rs2)
{
    int rdv, rs1v, rs2v;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    rs2v= registerValueDecimal[findPosition(rs2)];
    registerValueDecimal[rdv]= registerValueDecimal[rs1v] << rs2v ;
    registerValueBinary[rdv]= decimalToBinary(registerValueDecimal[rdv]);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);

}
void slliF(string rd, string rs1, string imm)
{
    int rdv, rs1v, immv;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    immv= stoi(imm);
    registerValueDecimal[rdv]= registerValueDecimal[rs1v] << immv ;
    registerValueBinary[rdv]= decimalToBinary(registerValueDecimal[rdv]);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);

}
void sraF(string rd, string rs1, string rs2)
{
    int rdv, rs1v, rs2v;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    rs2v= registerValueDecimal[findPosition(rs2)];
    registerValueDecimal[rdv]= registerValueDecimal[rs1v] >> rs2v ;
    registerValueBinary[rdv]= decimalToBinary(registerValueDecimal[rdv]);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);

}
void sraiF(string rd, string rs1, string imm)
{
    int rdv, rs1v, immv;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    immv= stoi(imm);
    registerValueDecimal[rdv]= registerValueDecimal[rs1v] >> immv ;
    registerValueBinary[rdv]= decimalToBinary(registerValueDecimal[rdv]);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);

}
void srlF(string rd, string rs1, string rs2)
{
    int rdv, rs1v, rs2v;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    rs2v= registerValueDecimal[findPosition(rs2)];
    bitset<32> bits = registerValueDecimal[rs1v] >> rs2v;
    if(rs2v >= 1){
     bitset<32> mask = 0x7fffffff >> (rs2v - 1);
     bits = bits & mask;
    }
    registerValueDecimal[rdv] = bits.to_ulong();
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
    registerValueBinary[rdv]=decimalToBinary(registerValueDecimal[rdv]);
}
void srliF(string rd, string rs1, string imm)
{
    int rdv, rs1v, immv;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    immv= stoi(imm);
    bitset<32> bits = registerValueDecimal[rs1v] >> immv;
    if(immv >= 1){
     bitset<32> mask = 0x7fffffff >> (immv - 1);
     bits = bits & mask;
    }
    registerValueDecimal[rdv] = bits.to_ulong();
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
    registerValueBinary[rdv]=decimalToBinary(registerValueDecimal[rdv]);
}
void sltF(string rd, string rs1, string rs2)
{
    int rdv, rs1v, rs2v;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    rs2v= findPosition(rs2);
    if (registerValueDecimal[rs1v]<registerValueDecimal[rs2v]) {
        registerValueDecimal[rdv]=1;
    }
    else if (registerValueDecimal[rs1v]>=registerValueDecimal[rs2v]) // is the equal sign right here
    {
        registerValueDecimal[rdv]=0;
    }
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
    registerValueBinary[rdv]=decimalToBinary(registerValueDecimal[rdv]);
}
void xorF(string rd, string rs1, string rs2)
{
    int rdv, rs1v, rs2v;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    rs2v= findPosition(rs2);
    string rs1Binary= registerValueBinary[rs1v];
    string rs2Binary= registerValueBinary[rs2v];
    string result="";
    int length, numberOfZeros;
    if (rs1Binary.length()>rs2Binary.length()) {
        length=rs1Binary.length();
        numberOfZeros=rs1Binary.length()-rs2Binary.length();
        for (int i=0; i<numberOfZeros; i++) {
            rs2Binary='0'+rs2Binary;
        }    }
    else
    {
        length=rs2Binary.length();
        numberOfZeros=rs2Binary.length()-rs1Binary.length();

        for (int i=0; i<numberOfZeros; i++) {
            rs1Binary='0'+rs1Binary;
        }
    }
    for (int i=0; i<length; i++) {
        if (rs1Binary[i]==rs2Binary[i]) {
            result=result+'0';
        }
        else if (rs1Binary[i]!=rs2Binary[i])
        {
            result=result+'1';
        }
    }
    registerValueBinary[rdv]= result;
    registerValueDecimal[rdv]= binaryToDecimal(result);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
    
}
void andF(string rd, string rs1, string rs2)
{
    int rdv, rs1v, rs2v;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    rs2v= findPosition(rs2);
    string rs1Binary= registerValueBinary[rs1v];
    string rs2Binary= registerValueBinary[rs2v];
    string result="";
    int length, numberOfZeros;
    if (rs1Binary.length()>rs2Binary.length()) {
        length=rs1Binary.length();
        numberOfZeros=rs1Binary.length()-rs2Binary.length();
        for (int i=0; i<numberOfZeros; i++) {
            rs2Binary='0'+rs2Binary;
        }
        
    }
    else
    {
        length=rs2Binary.length();
        numberOfZeros=rs2Binary.length()-rs1Binary.length();

        for (int i=0; i<numberOfZeros; i++) {
            rs1Binary='0'+rs1Binary;

        }
    }
    for (int i=0; i<length; i++) {
        if (rs1Binary[i]=='0' || rs2Binary[i]=='0') {
            result=result+'0';
        }
        else if (rs1Binary[i]=='1' && rs2Binary[i]=='1')
        {
            result=result+'1';
        }
    }
    registerValueBinary[rdv]= result;
    registerValueDecimal[rdv]= binaryToDecimal(result);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
}
void orF(string rd, string rs1, string rs2)
{
    int rdv, rs1v, rs2v;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    rs2v= findPosition(rs2);
    string rs1Binary= registerValueBinary[rs1v];
    string rs2Binary= registerValueBinary[rs2v];
    string result="";
    int length, numberOfZeros;
    if (rs1Binary.length()>rs2Binary.length()) {
        length=rs1Binary.length();
        numberOfZeros=rs1Binary.length()-rs2Binary.length();
        for (int i=0; i<numberOfZeros; i++) {
            rs2Binary='0'+rs2Binary;
        }    }
    else
    {
        length=rs2Binary.length();
        numberOfZeros=rs2Binary.length()-rs1Binary.length();

        for (int i=0; i<numberOfZeros; i++) {
            rs1Binary='0'+rs1Binary;
        }
    }
    for (int i=0; i<length; i++) {
        if (rs1Binary[i]=='0' && rs2Binary[i]=='0') {
            result=result+'0';
        }
        else if (rs1Binary[i]=='1' || rs2Binary[i]=='1')
        {
            result=result+'1';
        }
    }

    registerValueBinary[rdv]= result;
    registerValueDecimal[rdv]= binaryToDecimal(result);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
}

void sltuF(string rd, string rs1, string rs2){
    int rdv, rs1v, rs2v;
    rdv= findPosition(rd);
    rs1v= findPosition(rs1);
    rs2v= findPosition(rs2);
    
    unsigned int rs1U, rs2U;
    rs1U= (unsigned int)registerValueDecimal[rs1v];
    rs2U= (unsigned int)registerValueDecimal[rs2v];

    if (rs1U<rs2U) {
        registerValueDecimal[rdv]=1;
    }
    else if (rs1U>=rs2U) // is the equal sign right here
    {
        registerValueDecimal[rdv]=0;
    }
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
    registerValueBinary[rdv]=decimalToBinary(registerValueDecimal[rdv]);
}
void xoriI(string rs1,string rd,string imm)
{
    int rdv, rs1v,immv;
        rdv=findPosition(rd);
        rs1v=findPosition (rs1);
        immv=stoi(imm);
       
    
    
    //int rs= registerValue[rs1v][1]; //accessing the binary value of rs
    string rs= registerValueBinary[rs1v];
    
    //string str=to_string(rs); //converting integer to string
    string result=" ";
    for (int i=0; i < rs.length(); i++ )
    {
        //if (str[i]==0)
        if(rs[i]=='0')
            result=result+'1';
        
        if (rs[i]=='1')
            result=result+'0';
            
    }
    
    registerValueBinary[rdv]= result;
    registerValueDecimal[rdv]= binaryToDecimal(result);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
   // registerValueHexadecimal[rdv]=BinaryTohexadecimal(resultint)
     
    
}
void oriI(string rs1,string rd,string imm)
{
    int rdv, rs1v;
           rdv=findPosition(rd);
           rs1v=findPosition (rs1);
        
    
    string rs= registerValueBinary[rs1v]; //access the binary value of rs1
    
    string immediate = decimalToBinary(stoi(imm)); // converting the immediate value to binary
 
    string result=" ";
        int length, numberOfZeros;
        if (rs.length()>immediate.length()) {
            length=rs.length();
            numberOfZeros=rs.length()-immediate.length();
            for (int i=0; i<numberOfZeros; i++) {
                immediate='0'+immediate;
            }    }
        else
        {
            length=immediate.length();
            numberOfZeros=immediate.length()-rs.length();
     
            for (int i=0; i<numberOfZeros; i++) {
                rs='0'+rs;
            }
        }
        for (int i=0; i<length; i++) {
            if (rs[i]=='0' && immediate[i]=='0') {
                result=result+'0';
            }
            else if (rs[i]=='1' || immediate[i]=='1')
            {
                result=result+'1';
            }
        }
     
        registerValueBinary[rdv]= result;
        registerValueDecimal[rdv]= binaryToDecimal(result);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
    //registerValueHexadecimal[rdv]=binaryTohexadecimal(result);
    
   
 
}
void andiI(string rs1,string rd,string imm)
{
    int rdv, rs1v;
           rdv=findPosition(rd);
           rs1v=findPosition (rs1);
        
    
    string rs= registerValueBinary[rs1v]; //access the binary value of rs1
    
    string immediate = decimalToBinary(stoi(imm)); // converting the immediate value to binary
 
    string result=" ";
    int length, numberOfZeros;
    if (rs.length()>immediate.length()) {
        length=rs.length();
        numberOfZeros=rs.length()-immediate.length();
        for (int i=0; i<numberOfZeros; i++) {
            immediate='0'+immediate;
        }    }
    else
    {
        length=immediate.length();
        numberOfZeros=immediate.length()-rs.length();
 
        for (int i=0; i<numberOfZeros; i++) {
            rs='0'+rs;
        }
    }
    for (int i=0; i<length; i++) {
        if (rs[i]=='0' || immediate[i]=='0') {
            result=result+'0';
        }
        else if (rs[i]=='1' && immediate[i]=='1')
        {
            result=result+'1';
        }
    }
 
    registerValueBinary[rdv]= result;
    registerValueDecimal[rdv]=binaryToDecimal(registerValueBinary[rdv]);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
    //registerValueDecimal[rdv]= binaryToDecimal(result);
 
   // registerValueHexadecimal[rdv]=BinaryTohexadecimal(resultint)
}
void sltiI(string rs1,string rd,string imm)
{
    int rdv, rs1v,immv;
        rdv=findPosition(rd);
        rs1v=findPosition (rs1);
        immv=stoi(imm);
    int rs= registerValueDecimal[rs1v];
    
    if (rs<immv){
        registerValueDecimal[rdv]=1;
    }
        
    else if (rs>=immv) {
        registerValueDecimal[rdv]=0;
       
    }
  
    registerValueBinary[rdv]=decimalToBinary(registerValueDecimal[rdv]);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);

    // registerValueHexadecimal[rdv]=BinaryTohexadecimal(result);
}
void sltiuI(string rs1,string rd,string imm)
{
        int rdv, rs1v, immv;
        rdv= findPosition(rd);
        rs1v= findPosition(rs1);
        immv= findPosition(imm);
        
        unsigned int rs1U, immU;
        rs1U= (unsigned int)registerValueDecimal[rs1v];
        immU= (unsigned int)registerValueDecimal[immv];
     
        if (rs1U<immU) {
            registerValueDecimal[rdv]=1;
        }
        else if (rs1U>=immU) // is the equal sign right here
        {
            registerValueDecimal[rdv]=0;
        }
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
    registerValueBinary[rdv]=decimalToBinary(registerValueDecimal[rdv]);

}
void addiI(string rs1,string rd,string imm)
{ int rdv, rs1v,immv;
    rdv=findPosition(rd);
    rs1v=findPosition (rs1);
    immv=stoi(imm);
    
    registerValueDecimal[rdv]=registerValueDecimal[rs1v]+immv;
    registerValueBinary[rdv]=decimalToBinary(registerValueDecimal[rdv]);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);

    
}
void luiU( string rd, string imm)
{
    int rdv;
    rdv=findPosition(rd);
      
    int immediatev = stoi(imm);
    
    immediatev=immediatev<<12;
    registerValueDecimal[rdv]=immediatev;
    registerValueBinary[rdv]=decimalToBinary(immediatev);
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);

}


void auipcU(string rd, string imm)
{
    int rdv;
    rdv=findPosition(rd);
    int immediatev=stoi(imm);
    immediatev = immediatev<<12;
    registerValueDecimal[rdv] = immediatev+ProgramCounterValue;
    registerValueHexadecimal[rdv]= decimalToHex(registerValueDecimal[rdv]);
    registerValueBinary[rdv]=decimalToBinary(registerValueDecimal[rdv]);
}

string decimalToHex(int decimal)
{
    stringstream ss;
    ss << hex << decimal;
    string str = ss.str();
    return str;
}

