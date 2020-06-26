import json
with open('Opcodes.json') as json_file:
    data = json.load(json_file)

    for n in range(0, 256):

        string = ""
        op1 = ""
        op2 = ""
        strHex = ("0x%0.2X" % n)[:4]
        mnemonic = data['unprefixed'][strHex]['mnemonic']
        byte = data['unprefixed'][strHex]['bytes']
        cycles = data['unprefixed'][strHex]['cycles']
        if(data['unprefixed'][strHex]['operands']):
            op1 = data['unprefixed'][strHex]['operands'][0]['name']
            try:
                op2 = data['unprefixed'][strHex]['operands'][1]['name']
            except:
                pass

# u8 CPU::opcode0x00() {
# 	return u8();
# }
        string += 'u8 CPU::opcode'+strHex+'(){	// ' + mnemonic
        if(op1 != ""):
            string += " " + op1
        if(op2 != ""):
            string += " " + op2
        print(string)
        print('std::cout << ' + '"' + strHex +
              ' nicht implementiert" << std::endl;')
        print("     return 0; }")

    print("")
    print("")
    print("")
    for n in range(0, 256):

        string = ""
        op1 = ""
        op2 = ""
        strHex = ("0x%0.2X" % n)[:4]
        mnemonic = data['cbprefixed'][strHex]['mnemonic']
        byte = data['cbprefixed'][strHex]['bytes']
        cycles = data['cbprefixed'][strHex]['cycles']
        if(data['cbprefixed'][strHex]['operands']):
            op1 = data['cbprefixed'][strHex]['operands'][0]['name']
            try:
                op2 = data['cbprefixed'][strHex]['operands'][1]['name']
            except:
                pass

# u8 CPU::opcode0x00() {
# 	return u8();
# }
        string += 'u8 CPU::prefixedopcode'+strHex+'(){	// ' + mnemonic
        if(op1 != ""):
            string += " " + op1
        if(op2 != ""):
            string += " " + op2
        print(string)
        print('std::cout << ' + '"prefixed ' + strHex +
              ' nicht implementiert" << std::endl;')
        print("     return 0; }")
    print("")
    print("")
    print("")
