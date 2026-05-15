import pygame
import random
import sys
import time

pygame.init()

class Minesweeper:
    """扫雷游戏主类"""
    
    # 游戏常量
    CELL_SIZE = 50
    GRID_WIDTH = 9
    GRID_HEIGHT = 9
    MINES_COUNT = 10
    MARGIN = 50
    INFO_HEIGHT = 80
    FPS = 60
    
    # 颜色定义
    COLORS = {
        'background': (240, 240, 240),
        'grid_bg': (200, 200, 200),
        'cell_hidden': (180, 180, 180),
        'cell_revealed': (200, 200, 200),
        'text': (0, 0, 0),
        'mine': (255, 0, 0),
        'flag': (0, 255, 0),
        'info_bg': (50, 50, 50),
        'info_text': (255, 255, 255),
        'button_normal': (0, 150, 0),
        'button_hover': (0, 200, 0),
        'border': (0, 0, 0),
    }
    
    # 数字颜色
    NUMBER_COLORS = {
        1: (0, 0, 255),      # 蓝色
        2: (0, 128, 0),      # 绿色
        3: (255, 0, 0),      # 红色
        4: (0, 0, 128),      # 深蓝
        5: (128, 0, 0),      # 深红
        6: (0, 128, 128),    # 青色
        7: (0, 0, 0),        # 黑色
        8: (128, 128, 128)   # 灰色
    }
    
    def __init__(self):
        """初始化游戏"""
        # 计算窗口尺寸
        self.window_width = self.CELL_SIZE * self.GRID_WIDTH + self.MARGIN * 2
        self.window_height = self.CELL_SIZE * self.GRID_HEIGHT + self.MARGIN * 2 + self.INFO_HEIGHT
        
        # 创建游戏窗口
        self.screen = pygame.display.set_mode((self.window_width, self.window_height))
        pygame.display.set_caption("Minesweeper")
        
        # 字体：优先使用系统中文字体，回退到默认字体
        font_names = ['SimHei', 'SimSun', 'microsoftyahei', 'simsun', 'arial']
        self.font = None
        self.large_font = None
        
        for font_name in font_names:
            try:
                self.font = pygame.font.SysFont(font_name, 30)
                self.large_font = pygame.font.SysFont(font_name, 40)
                break
            except:
                continue
        
        # 如果所有字体都失败，使用默认字体
        if self.font is None:
            self.font = pygame.font.Font(None, 30)
            self.large_font = pygame.font.Font(None, 40)
        
        # 游戏时钟
        self.clock = pygame.time.Clock()
        
        # 重启按钮
        self.restart_button = None
        
        self.reset_game()
        
    def reset_game(self):
        """重置游戏状态"""
        self.board = [[0 for _ in range(self.GRID_WIDTH)] for _ in range(self.GRID_HEIGHT)]
        self.revealed = [[False for _ in range(self.GRID_WIDTH)] for _ in range(self.GRID_HEIGHT)]
        self.flagged = [[False for _ in range(self.GRID_WIDTH)] for _ in range(self.GRID_HEIGHT)]
        self.mines = set()
        self.game_over = False
        self.game_won = False
        self.steps = 0
        self.start_time = time.time()
        
        self.place_mines()
        self.calculate_numbers()
        
    def place_mines(self):
        """高效地放置地雷 - 使用random.sample替代while循环"""
        self.mines.clear()
        all_cells = [(r, c) for r in range(self.GRID_HEIGHT) for c in range(self.GRID_WIDTH)]
        self.mines = set(random.sample(all_cells, self.MINES_COUNT))
            
    def calculate_numbers(self):
        """计算每个格子周围的地雷数"""
        for row in range(self.GRID_HEIGHT):
            for col in range(self.GRID_WIDTH):
                if (row, col) in self.mines:
                    self.board[row][col] = -1  # -1 表示地雷
                else:
                    self.board[row][col] = self._count_adjacent_mines(row, col)
    
    def _count_adjacent_mines(self, row, col):
        """计算相邻的地雷数"""
        count = 0
        for r in range(max(0, row - 1), min(self.GRID_HEIGHT, row + 2)):
            for c in range(max(0, col - 1), min(self.GRID_WIDTH, col + 2)):
                if (r, c) in self.mines:
                    count += 1
        return count
                    
    def reveal(self, row, col):
        """揭示格子"""
        if not (0 <= row < self.GRID_HEIGHT and 0 <= col < self.GRID_WIDTH):
            return
            
        if self.revealed[row][col] or self.flagged[row][col]:
            return
            
        self.revealed[row][col] = True
        self.steps += 1
        
        # 如果是地雷，游戏结束
        if (row, col) in self.mines:
            self.game_over = True
            return
            
        # 如果是空白格子，递归揭示周围的格子
        if self.board[row][col] == 0:
            self._reveal_adjacent(row, col)
                        
        # 检查是否获胜
        self.check_win()
    
    def _reveal_adjacent(self, row, col):
        """递归揭示相邻格子"""
        for r in range(max(0, row - 1), min(self.GRID_HEIGHT, row + 2)):
            for c in range(max(0, col - 1), min(self.GRID_WIDTH, col + 2)):
                if not self.revealed[r][c]:
                    self.revealed[r][c] = True
                    if self.board[r][c] == 0:
                        self._reveal_adjacent(r, c)
        
    def toggle_flag(self, row, col):
        """切换标记旗帜"""
        if not (0 <= row < self.GRID_HEIGHT and 0 <= col < self.GRID_WIDTH):
            return
            
        if not self.revealed[row][col]:
            self.flagged[row][col] = not self.flagged[row][col]
            self.steps += 1
            
    def check_win(self):
        """检查是否获胜"""
        for row in range(self.GRID_HEIGHT):
            for col in range(self.GRID_WIDTH):
                if (row, col) not in self.mines and not self.revealed[row][col]:
                    return
                    
        self.game_won = True
        self.game_over = True
        
    def draw(self):
        """绘制游戏画面"""
        self.screen.fill(self.COLORS['background'])
        
        # 绘制信息区域
        pygame.draw.rect(self.screen, self.COLORS['info_bg'], (0, 0, self.window_width, self.INFO_HEIGHT))
        self._draw_info()
        
        # 绘制游戏网格
        self._draw_grid()
        
        # 绘制重新开始按钮
        self.restart_button = self._draw_restart_button()
        
    def _draw_info(self):
        """绘制游戏信息（地雷数、步数、时间）"""
        mines_left = self.MINES_COUNT - sum(sum(row) for row in self.flagged)
        time_elapsed = int(time.time() - self.start_time)
        
        info_text = f"Mines left: {mines_left}  Steps: {self.steps}  Time: {time_elapsed}s"
        text_surface = self.font.render(info_text, True, self.COLORS['info_text'])
        self.screen.blit(text_surface, (20, 20))
        
        # 显示游戏状态
        if self.game_over:
            status_text = "Game Over!" if not self.game_won else "You Win!"
            status_surface = self.large_font.render(status_text, True, (255, 255, 0))
            self.screen.blit(status_surface, (self.window_width // 2 - status_surface.get_width() // 2, 
                                              self.INFO_HEIGHT // 2 - status_surface.get_height() // 2))
    
    def _draw_grid(self):
        """绘制游戏网格"""
        for row in range(self.GRID_HEIGHT):
            for col in range(self.GRID_WIDTH):
                self._draw_cell(row, col)
    
    def _draw_cell(self, row, col):
        """绘制单个格子"""
        x = self.MARGIN + col * self.CELL_SIZE
        y = self.INFO_HEIGHT + self.MARGIN + row * self.CELL_SIZE
        
        # 绘制格子背景
        bg_color = self.COLORS['cell_revealed'] if self.revealed[row][col] else self.COLORS['cell_hidden']
        pygame.draw.rect(self.screen, bg_color, (x, y, self.CELL_SIZE, self.CELL_SIZE))
        
        # 绘制格子边框
        pygame.draw.rect(self.screen, self.COLORS['border'], (x, y, self.CELL_SIZE, self.CELL_SIZE), 1)
        
        # 绘制格子内容
        if self.revealed[row][col]:
            if (row, col) in self.mines:
                pygame.draw.circle(self.screen, self.COLORS['mine'], 
                                  (x + self.CELL_SIZE // 2, y + self.CELL_SIZE // 2), 
                                  self.CELL_SIZE // 3)
            elif self.board[row][col] > 0:
                number_text = str(self.board[row][col])
                color = self.NUMBER_COLORS[self.board[row][col]]
                number_surface = self.font.render(number_text, True, color)
                self.screen.blit(number_surface, 
                               (x + self.CELL_SIZE // 2 - number_surface.get_width() // 2, 
                                y + self.CELL_SIZE // 2 - number_surface.get_height() // 2))
        elif self.flagged[row][col]:
            pygame.draw.polygon(self.screen, self.COLORS['flag'], [
                (x + self.CELL_SIZE // 2, y + 10),
                (x + self.CELL_SIZE // 2, y + self.CELL_SIZE - 10),
                (x + self.CELL_SIZE - 10, y + self.CELL_SIZE // 2)
            ])
    
    def _draw_restart_button(self):
        """绘制重新开始按钮"""
        button = pygame.Rect(self.window_width - 120, self.INFO_HEIGHT // 2 - 15, 100, 30)
        pygame.draw.rect(self.screen, self.COLORS['button_normal'], button)
        restart_text = self.font.render("Restart", True, (255, 255, 255))
        self.screen.blit(restart_text, (button.centerx - restart_text.get_width() // 2, 
                                        button.centery - restart_text.get_height() // 2))
        return button
    
    def handle_events(self):
        """处理游戏事件"""
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return False
            
            if event.type == pygame.MOUSEBUTTONDOWN:
                mouse_x, mouse_y = event.pos
                
                # 检查是否点击了重启按钮
                if self.restart_button and self.restart_button.collidepoint(mouse_x, mouse_y):
                    self.reset_game()
                    continue
                
                # 计算点击的格子位置
                col = (mouse_x - self.MARGIN) // self.CELL_SIZE
                row = (mouse_y - self.INFO_HEIGHT - self.MARGIN) // self.CELL_SIZE
                
                # 检查是否在有效范围内
                if 0 <= row < self.GRID_HEIGHT and 0 <= col < self.GRID_WIDTH:
                    if event.button == 1:  # 左键
                        self.reveal(row, col)
                    elif event.button == 3:  # 右键
                        self.toggle_flag(row, col)
            
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_r:  # R键重新开始
                    self.reset_game()
        
        return True
    
    def run(self):
        """主游戏循环"""
        running = True
        while running:
            running = self.handle_events()
            self.draw()
            pygame.display.flip()
            self.clock.tick(self.FPS)
        
        pygame.quit()
        sys.exit()


# 主程序入口
if __name__ == "__main__":
    game = Minesweeper()
    game.run()