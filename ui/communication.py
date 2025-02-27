"""

"""

import ctypes

# Tải thư viện DLL
lib = ctypes.CDLL(r"..\mcts\lib\libmcts.dll", winmode=0)


class Board:
    def __init__(self, _turn):
        self.entry_size = 3  # (x, y, player) = 3 bytes
        self.board = {}  # Using a dictionary to store the board
        self._turn = _turn  # Turn of the player

    def get_piece(self, x: int, y: int):
        """Get the piece at (x, y)"""
        return self.board.get((x, y), None)

    def add_piece(self, x: int, y: int, player: int):
        """Add a piece to the board"""
        if (x, y) not in self.board:
            self.board[(x, y)] = player
            return True  # Successfully added
        return False  # Failed to add

    def check_winner(self):
        """Check if there is a winner"""
        directions = [(0, 1), (1, 0), (1, 1), (1, -1)]
        for (x, y), player in self.board.items():
            for dx, dy in directions:
                count = 1
                for i in range(1, 5):
                    new_x, new_y = x + dx * i, y + dy * i
                    if self.get_piece(new_x, new_y) == player:
                        count += 1
                    else:
                        break

                if count >= 5:
                    return player
        return None

    def clear(self):
        """Clear the board"""
        self.board.clear()

    def dict_to_array(self):
        """Transport dictionary to array"""
        arr = []
        for (x, y), player in self.board.items():
            arr.extend([x, y, player])  # Chuyển từng phần tử thành list [x, y, player]
        return  arr

    def next_move(self, last_move_x, last_move_y):
        """Get the next move"""
        last_move = (last_move_x << 8) | last_move_y  # Encode the last move
        array = self.dict_to_array()
        array = (ctypes.c_int * len(array))(*array)  # Convert the list to a C array

        # uint16_t decision(int* arr, int size, int last_move)
        lib.decision.argtypes = (ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int)
        lib.decision.restype = ctypes.c_int  # Return a single integer (actually 2 bytes)

        pos = lib.decision(array, len(array), last_move)  # Call the MCTS function (C++

        x = pos >> 8  # Get the x coordinate
        y = pos & 0xFF  # Get the y coordinate

        # Default machine uses 0 and player uses 1
        self.board[(x, y)] = 0  # Add the piece to the board

        # Successfull move
        self._turn["turn"] = 1
