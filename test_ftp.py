#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import socket

from typing import Optional, Tuple

HOST = "127.0.0.1"
PORT = 4242


class Test:
    HOME_PATH = "./"

    NAME = "Test"

    def __init__(self):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.connect((HOST, PORT))
        data = self.recv()
        if self.parse_code(data) != 220:
            raise Exception("Connection failed")

    def send(self, data) -> None:
        self._socket.send(data)

    def send_command(self, line: str) -> None:
        self.send(line.encode("utf-8") + b"\r\n")

    def recv(self) -> bytes:
        res = self._socket.recv(16384)
        return res

    def parse_code(self, data) -> int:
        try:
            return int(data[:3])
        except ValueError:
            return -1

    def run_test(self) -> bool:
        """
        Runs the test.
        Returns True if the test passed, False otherwise.
        """
        raise NotImplementedError


class TestConnection(Test):
    NAME = "Test connection"

    def run_test(self) -> bool:
        return True


class TestUser(Test):
    NAME = "Test USER command"

    def run_test(self) -> bool:
        self.send_command("USER Anonymous")
        data = self.recv()
        if self.parse_code(data) != 331:
            print("Test failed: USER command did not return 331")
            return False
        return True


class TestPass(Test):
    NAME = "Test PASS command"

    def run_test(self) -> bool:
        if not TestUser.run_test(self):
            return False

        self.send_command("PASS ")
        data = self.recv()
        if self.parse_code(data) != 230:
            print("Test failed: PASS command did not return 230")
            return False

        return True


class TestPwd1(Test):
    NAME = "Test PWD basic"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False

        self.send_command("PWD")
        data = self.recv()
        if self.parse_code(data) != 257:
            print("Test failed: PWD command did not return 257")
            return False
        if not "is current directory" in data.decode("utf-8"):
            print(
                "Test failed: PWD command did not return the correct message",
                file=sys.stderr,
            )
            return False
        return True


class TestPwd2(Test):
    NAME = "Test PWD while not logged in"

    def run_test(self) -> bool:
        self.send_command("PWD")
        data = self.recv()
        code = self.parse_code(data)
        if code == 257:
            print("Test failed: PWD command should not return 257")
            return False
        elif code != 530:
            print("Test failed: PWD command did not return 530")
            return False
        return True


class TestNOOP(Test):
    NAME = "Test NOOP command"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False

        self.send_command("NOOP")
        data = self.recv()
        if self.parse_code(data) != 200:
            print("Test failed: NOOP command did not return 200")
            return False

        return True


class TestCwd1(Test):
    NAME = "Test CWD while not logged in"

    def run_test(self) -> bool:
        self.send_command("CWD")
        data = self.recv()
        code = self.parse_code(data)
        if code == 250:
            print("Test failed: CWD command should not return 250")
            return False
        elif code != 530:
            print("Test failed: CWD command did not return 530")
            return False
        return True


class TestCwd2(Test):
    NAME = "Test CWD basic"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False

        self.send_command("CWD ./tests")
        data = self.recv()
        code = self.parse_code(data)
        if code != 250:
            print("Test failed: CWD command did not return 250")
            return False

        self.send_command("PWD")
        data = self.recv()
        code = self.parse_code(data)
        if code != 257:
            print("Test failed: PWD command did not return 257")
            return False

        if not '/tests"' in data.decode("utf-8"):
            print(
                "Test failed: PWD command did not return the correct path",
                file=sys.stderr,
            )
            return False

        return True


class TestCDUP(Test):
    NAME = "Test CDUP command"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False

        self.send_command("CWD ./tests")
        data = self.recv()
        code = self.parse_code(data)
        if code != 250:
            print("Test failed: CWD command did not return 250")
            return False

        self.send_command("CDUP")
        data = self.recv()
        code = self.parse_code(data)
        if code != 200:
            print("Test failed: CDUP command did not return 200")
            return False

        self.send_command("PWD")
        data = self.recv()
        code = self.parse_code(data)
        if code != 257:
            print("Test failed: PWD command did not return 257")
            return False

        return True


class TestQuit(Test):
    NAME = "Test QUIT command"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False

        self.send_command("QUIT")
        data = self.recv()
        code = self.parse_code(data)
        if code != 221:
            print("Test failed: QUIT command did not return 221")
            return False

        return True


class TestQuit2(Test):
    NAME = "Test QUIT command while not logged in"

    def run_test(self) -> bool:
        self.send_command("QUIT")
        data = self.recv()
        code = self.parse_code(data)
        if code != 221:
            print("Test failed: QUIT command did not return 221")
            return False

        return True


class TestHelp(Test):
    NAME = "Test HELP command"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False

        self.send_command("HELP")
        data = self.recv()
        code = self.parse_code(data)
        if code != 214:
            print("Test failed: HELP command did not return 214")
            return False

        if "The following commands are recognized" not in data.decode("utf-8"):
            print(
                "Test failed: HELP command did not return the correct message",
                file=sys.stderr,
            )
            return False
        return True


class TestHelp2(Test):
    NAME = "Test HELP command with argument"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False

        self.send_command("HELP USER")
        data = self.recv()
        code = self.parse_code(data)

        if code != 214:
            print("Test failed: HELP command did not return 214")
            return False
        
        if "The following commands are recognized" in data.decode("utf-8"):
            print(
                "Test failed: HELP command returned the same message as without argument",
                file=sys.stderr,
            )
            return False

        return True
    

class TestPasv(Test):
    NAME = "Test PASV command"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False

        self.send_command("PASV")
        data = self.recv()
        code = self.parse_code(data)
        if code != 227:
            print("Test failed: PASV command did not return 227")
            return False

        return True


class TestPasv2(Test):
    NAME = "Test PASV command without being logged in"

    def run_test(self) -> bool:
        self.send_command("PASV")
        data = self.recv()
        code = self.parse_code(data)
        if code != 530:
            print("Test failed: PASV command did not return 530")
            return False

        return True

class TestDele(Test):
    NAME = "Test DELE command"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False
        
        with open("test.txt", "w") as f:
            f.write("test")
        self.send_command("DELE ./test.txt")
        data = self.recv()
        code = self.parse_code(data)
        os.system("rm test.txt -f")
        if code != 250:
            print("Test failed: DELE command did not return 250")
            return False

        return True


class TestDele2(Test):
    NAME = "Test DELE command with a file that does not exist"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False

        os.system("rm test.txt -f")
        self.send_command("DELE ./test.txt")
        data = self.recv()
        code = self.parse_code(data)
        if code != 550:
            print("Test failed: DELE command did not return 550")
            return False

        return True
    

class TestDele3(Test):
    NAME = "Test DELE command without being logged in"

    def run_test(self) -> bool:
        os.system("rm test.txt -f")
        self.send_command("DELE ./test.txt")
        data = self.recv()
        code = self.parse_code(data)
        if code != 530:
            print("Test failed: DELE command did not return 530")
            return False

        return True


class TestBufferManagment(Test):
    NAME = "Test USER command splitted in multiple chunks of data"

    def run_test(self) -> bool:
        self.send(b"US")
        self.send(b"ER Ano")
        self.send(b"nymous\r\n")
        data = self.recv()
        if self.parse_code(data) != 331:
            print("Test failed: USER command did not return 331")
            return False
        return True


class TestPasvABC(Test):
    def parse_pasv(self, data: bytes) -> Optional[Tuple[str, int]]:
        data = data.decode("utf-8")
        data = data.split("(")[1].split(")")[0].split(",")
        if len(data) != 6:
            return None
        try:
            port = int(data[4]) * 256 + int(data[5])
            if port < 0 or port > 65535:
                return None
        except ValueError:
            return None
        return ".".join(data[:4]), port


class TestPasv(TestPasvABC):
    NAME = "Test PASV command"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False

        self.send_command("PASV")
        data = self.recv()
        code = self.parse_code(data)
        if code != 227:
            print("Test failed: PASV command did not return 227")
            return False
        
        if not self.parse_pasv(data):
            print("Test failed: PASV command did not return a valid address")
            return False

        return True
    

class TestPasv2(TestPasvABC):
    NAME = "Test PASV command without being logged in"

    def run_test(self) -> bool:
        self.send_command("PASV")
        data = self.recv()
        code = self.parse_code(data)
        if code != 530:
            print("Test failed: PASV command did not return 530")
            return False

        return True
    

class TestPasv3(TestPasvABC):
    NAME = "Test PASV socket"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False

        self.send_command("PASV")
        data = self.recv()
        code = self.parse_code(data)
        if code != 227:
            print("Test failed: PASV command did not return 227")
            return False
        
        try:
            ip, port = self.parse_pasv(data)
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((ip, port))
            s.close()

        except ValueError:
            print("Test failed: PASV command did not return a valid address")
            return False

        return True


class TestPortABC(Test):
    def start_socket(self, ip, port) -> socket.socket:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((ip, port))
        s.listen(1)
        return s
    

class TestPort(TestPortABC):
    NAME = "Test PORT command"

    def run_test(self) -> bool:
        socket = self.start_socket("127.0.0.1", 4269)
    
        if not TestPass.run_test(self):
            return False

        self.send_command("PORT 127,0,0,1,16,173")
        data = self.recv()
        code = self.parse_code(data)

        if code != 200:
            print("Test failed: PORT command did not return 200")
            return False
        
        socket.close()

        return True
    

class TestPort2(TestPortABC):
    NAME = "Test PORT command without being logged in"

    def run_test(self) -> bool:
        self.send_command("PORT 127,0,0,1,16,173")
        data = self.recv()
        code = self.parse_code(data)

        if code != 530:
            print("Test failed: PORT command did not return 530")
            return False
        
        return True
    

class TestPort3(TestPortABC):
    NAME = "Test PORT without socket"

    def run_test(self) -> bool:
        if not TestPass.run_test(self):
            return False
        
        self.send_command("PORT 127,0,0,1,16,173")
        data = self.recv()
        code = self.parse_code(data)
        
        if code != 425:
            print("Test failed: PORT command did not return 425")
            return False
        
        return True

TESTS = [
    TestConnection,
    TestUser,
    TestPass,
    TestPwd1,
    TestPwd2,
    TestNOOP,
    TestCwd1,
    TestCwd2,
    TestCDUP,
    TestQuit,
    TestQuit2,
    TestHelp,
    TestHelp2,
    TestPasv,
    TestPasv2,
    TestDele,
    TestDele2,
    TestDele3,
    TestBufferManagment,
    TestPasv,
    TestPasv2,
    TestPasv3,
    TestPort,
    TestPort2,
    TestPort3,
]


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 test.py <ip> <port>")
        sys.exit(1)

    HOST = sys.argv[1]
    PORT = int(sys.argv[2])

    passed = 0
    failed = 0
    for test in TESTS:
        print("----", test.NAME, "----")
        try:
            t = test()
        except Exception as e:
            print(f"{test.NAME} failed to initialize: {e}")
            failed += 1
            print()
            continue
        if t.run_test():
            print(f"{t.NAME} passed")
            passed += 1
        else:
            print(f"{t.NAME} failed")
            failed += 1

        print()

    print(f"{passed} tests passed, {failed} tests failed")
    if failed > 0:
        sys.exit(1)
