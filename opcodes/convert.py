import json
#   mnemonic,opcodeHEX,bytes,cycles
# {"NOP", &CPU::OPCODEONE, 2, 2},
with open('Opcodes.json') as json_file:
    data = json.load(json_file)

    # UNPREFIXED DATA
    for n in range(0, 257):
        string = ""
        op1 = ""
        op2 = ""
        strHex = ("0x%0.2X" % n)[:4]
        mnemonic = data['unprefixed'][strHex]['mnemonic']
        byte = data['unprefixed'][strHex]['bytes']
        cycles = data['unprefixed'][strHex]['cycles']
        try:
            op1 = data['unprefixed'][strHex]['operands'][0]['name']
        except:
            pass
        try:
            op2 = data['unprefixed'][strHex]['operands'][1]['name']
        except:
            pass
        string += '{' + '\"' + mnemonic
        if(op1 != ""):
            string += " " + op1
        if(op2 != ""):
            string += " " + op2
        string += '\", ' + '&CPU::opcode'+strHex + ', ' + \
            str(byte) + ", " + str(int(cycles[0]/4)) + "},"
        print(string)

    print("")
    print("")
    print("")
    # PREFIXED DATA
    for n in range(0, 257):
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
        string += '{' + '\"' + mnemonic
        if(op1 != ""):
            string += " " + op1
        if(op2 != ""):
            string += " " + op2
        string += '\", ' + '&CPU::prefixedopcode'+strHex + \
            ', ' + str(byte) + ", " + str(int(cycles[0]/4)) + "},"
        print(string)

    print("")
    print("")
    print("")

    for n in range(0, 257):
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

        string += 'u8 opcode'+strHex+'();	// ' + mnemonic
        if(op1 != ""):
            string += " " + op1
        if(op2 != ""):
            string += " " + op2
        print(string)
    print("")
    print("")
    print("")

    for n in range(0, 257):
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

        string += 'u8 prefixedopcode'+strHex+'();	// ' + mnemonic
        if(op1 != ""):
            string += " " + op1
        if(op2 != ""):
            string += " " + op2
        print(string)
