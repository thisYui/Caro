import pygame
from communication import Board
import threading


class StartGame:
    def __init__(self):
        # System parameters
        self.screen_size_standard = 400
        self.caption = "Caro Game"

        # Interface parameters
        self.in_game = False

        # Pictures
        self.start_bg = pygame.image.load(r"assets/start_bg.png")
        self.start_button_img = pygame.image.load(r"assets/start_button.png")
        self.icon = pygame.image.load(r"assets/icon.png")

        # Calculate the new size of the background image
        original_width, original_height = self.start_bg.get_size()
        self.screen_width = self.screen_size_standard
        self.screen_height = int(self.screen_size_standard * original_height / original_width)

        start_button_width, start_button_height = self.start_button_img.get_size()
        self.start_button_width = int(self.screen_size_standard * start_button_width / original_width)
        self.new_start_button_height = int(self.screen_size_standard * start_button_height / original_width)

        # Screen
        self.screen = pygame.display.set_mode((self.screen_width, self.screen_height))

        # Scale
        self.start_bg = pygame.transform.scale(self.start_bg, (self.screen_width, self.screen_height))
        self.start_button_img = pygame.transform.scale(self.start_button_img,
                                                       (self.start_button_width, self.new_start_button_height))

        # Button
        self.start_button_x = (self.screen_width - self.start_button_width) // 2
        self.start_button_y = 4 * (self.screen_height // 5) + 25

    def draw_start_screen(self):
        """Draw the start screen."""
        self.screen.fill((0, 0, 0))
        self.screen.blit(self.start_bg, (0, 0))
        self.screen.blit(self.start_button_img, (self.start_button_x, self.start_button_y))

    def run(self):
        """Run the game."""
        pygame.init()
        pygame.display.set_caption(self.caption)
        pygame.display.set_icon(self.icon)

        while self.in_game is False:
            self.draw_start_screen()  # Draw the start screen

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    exit(0)

                elif event.type == pygame.MOUSEBUTTONDOWN:
                    if event.button == 1:
                        x, y = pygame.mouse.get_pos()
                        if (self.start_button_x <= x <= self.start_button_x + self.start_button_width and
                                self.start_button_y <= y <= self.start_button_y + self.new_start_button_height):
                            self.in_game = True

            pygame.display.flip()  # Update the screen

        pygame.quit()  # Quit the game


class GameBoard:
    def __init__(self):
        # System parameters
        self.screen_width, self.screen_height = 800, 600
        self.caption = "Caro Game"

        # Interface parameters
        self.screen = pygame.display.set_mode((self.screen_width, self.screen_height))
        self.running = True
        self._turn = {"turn": 1}  # 1 = X, 0 = O

        # Size of the cell and chess pieces
        self.max_size = 256  # Size of 1 byte
        self.cell_size = 50  # Size of each cell
        self.piece_scale = 0.8  # Scale chess pieces
        self.board_size = 256  # The number of cells in each row and column
        self.piece_size = int(self.cell_size * self.piece_scale)

        # Pictures
        self.icon = pygame.image.load(r"assets/icon.png")
        self.cell_img = pygame.image.load(r"assets/cell.png")
        self.o_img = pygame.transform.scale(pygame.image.load(r"assets/O.png"), (self.piece_size, self.piece_size))
        self.x_img = pygame.transform.scale(pygame.image.load(r"assets/X.png"), (self.piece_size, self.piece_size))
        self.reset_img = pygame.transform.scale(
            pygame.image.load(r"assets/reset_button.png"), (self.piece_size, self.piece_size))
        self.win_img = pygame.transform.scale(
            pygame.image.load(r"assets/win.png"), (7 * 0.1 * self.screen_width, 2 * 0.1 * self.screen_height))
        self.lose_img = pygame.transform.scale(
            pygame.image.load(r"assets/lose.png"), (7 * 0.1 * self.screen_width, 2 * 0.1 * self.screen_height))

        # Chess board management
        self.offset_x, self.offset_y = -128 * self.cell_size, -128 * self.cell_size  # Offset of the board
        self.dragging = False  # Dragging the board
        self.last_mouse_x, self.last_mouse_y = 0, 0

        # Limit the offset
        self.max_offset_x = -(self.board_size * self.cell_size - self.screen_width)
        self.max_offset_y = -(self.board_size * self.cell_size - self.screen_height)
        self.min_offset_x, self.min_offset_y = 0, 0

        # Button
        self.reset_button_x = self.screen_width - 1.5 * self.cell_size
        self.reset_button_y = 0.5 * self.cell_size

        # Board
        self.last_move_x, self.last_move_y = 0, 0
        self.board = Board(self._turn)

    def draw_board(self):
        """Draw board with dynamic offset."""
        visible_cols = self.screen_width // self.cell_size + 2
        visible_rows = self.screen_height // self.cell_size + 2

        start_col = max(0, -self.offset_x // self.cell_size)
        start_row = max(0, -self.offset_y // self.cell_size)

        for row in range(start_row, min(start_row + visible_rows, self.board_size)):
            for col in range(start_col, min(start_col + visible_cols, self.board_size)):
                draw_x = col * self.cell_size + self.offset_x
                draw_y = row * self.cell_size + self.offset_y

                self.screen.blit(self.cell_img, (draw_x, draw_y))

                # Identify the center of the cell
                center_x = draw_x + self.cell_size // 2
                center_y = draw_y + self.cell_size // 2

                if self.board.get_piece(row, col) == 1:  # X
                    rect = self.x_img.get_rect(center=(center_x, center_y))
                    self.screen.blit(self.x_img, rect.topleft)
                elif self.board.get_piece(row, col) == 0:  # O
                    rect = self.o_img.get_rect(center=(center_x, center_y))
                    self.screen.blit(self.o_img, rect.topleft)

    def reset_screen(self):
        self.screen.fill((255, 255, 255))  # Delete the screen
        self.draw_board()  # Draw the board
        self.screen.blit(self.reset_img, (self.reset_button_x, self.reset_button_y))

    def reset_params(self):
        self._turn["turn"] = 1
        self.board.clear()

    def check_win(self):
        check = self.board.check_winner()
        if check is not None:
            pygame.display.flip()  # Update the screen
            self.reset_screen()  # Reset the screen
            if check == 1:
                self.screen.blit(self.win_img, (self.screen_width // 2 - 3.5 * 0.1 * self.screen_width,
                                                self.screen_height // 2 - 0.4 * self.screen_height))
            elif check == 0:
                self.screen.blit(self.lose_img, (self.screen_width // 2 - 3.5 * 0.1 * self.screen_width,
                                                 self.screen_height // 2 - 0.4 * self.screen_height))
            pygame.display.flip()  # Update the screen

            while self.running:
                event = pygame.event.wait()  # Chờ sự kiện xảy ra

                if event.type == pygame.QUIT:
                    self.running = False
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    if event.button == 1:
                        self.reset_params()
                        break
            return True
        
        else:
            return False

    def run(self):
        """Run the game."""
        pygame.init()
        pygame.display.set_caption(self.caption)
        pygame.display.set_icon(self.icon)

        while self.running:
            self.reset_screen()  # Reset the screen
            self.check_win()

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False

                elif event.type == pygame.MOUSEBUTTONDOWN:
                    if event.button == 1:  # Click the left mouse button to add a piece
                        if self._turn["turn"] == 0:
                            # Waitting for the next move because the AI is thinking
                            continue

                        x, y = pygame.mouse.get_pos()
                        col = (x - self.offset_x) // self.cell_size
                        row = (y - self.offset_y) // self.cell_size

                        if (self.reset_button_x <= x <= self.reset_button_x + self.piece_size and
                                self.reset_button_y <= y <= self.reset_button_y + self.piece_size):
                            self.reset_params()
                        elif (0 <= row < self.board_size and
                              0 <= col < self.board_size and
                              self.board.get_piece(row, col) is None):
                            self.board.add_piece(row, col, self._turn["turn"])
                            self._turn["turn"] = 0

                            # Call the next move
                            if not self.check_win():
                                threading.Thread(target=self.board.next_move, args=(row, col)).start()

                    elif event.button == 3:  # Click the right mouse button to move the board
                        self.dragging = True
                        self.last_mouse_x, self.last_mouse_y = pygame.mouse.get_pos()

                elif event.type == pygame.MOUSEBUTTONUP:
                    if event.button == 3:  # Release the right mouse button
                        self.dragging = False

                elif event.type == pygame.MOUSEMOTION and self.dragging:
                    # Get the current position of the mouse
                    mouse_x, mouse_y = pygame.mouse.get_pos()

                    # Update the offset
                    self.offset_x += mouse_x - self.last_mouse_x
                    self.offset_y += mouse_y - self.last_mouse_y
                    self.offset_x = max(self.max_offset_x, min(self.min_offset_x, self.offset_x))
                    self.offset_y = max(self.max_offset_y, min(self.min_offset_y, self.offset_y))

                    # Update the last position of the mouse
                    self.last_mouse_x, self.last_mouse_y = mouse_x, mouse_y

            pygame.display.flip()  # Update the screen

        pygame.quit()  # Quit the game
