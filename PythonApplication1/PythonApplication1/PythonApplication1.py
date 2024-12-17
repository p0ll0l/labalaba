
class BlockSpace:
    def __init__(self, file_path):
        self.file_path = file_path
        self.block_size = 0
        self.num_blocks = 0
        self.free_blocks = set()  
        self.transaction_cache = [] 
        self.transaction_in_progress = False  

    def initialize(self, block_size, num_blocks):
        if not (1024 <= block_size <= 65536 and (block_size & (block_size - 1)) == 0):
            raise ValueError("������ ����� ������ ���� �������� 2 � ������ � �������� �� 1 �� �� 64 ��.")

        if num_blocks > (2 ** 32 - 1):
            raise ValueError("���������� ������ �� ������ ��������� 2^32 - 1.")

        self.block_size = block_size
        self.num_blocks = num_blocks

        try:
            with open(self.file_path, "wb") as f:
                f.write(b"\x00" * (block_size * num_blocks))  
            self.free_blocks = set(range(num_blocks))  
            print(f"������������� �������. {self.num_blocks} ������ �� {self.block_size} ����.")
        except OSError as e:
            print(f"������ ��� ������ � ������: {e}")
            raise

    def allocate_blocks(self, count, preferred_blocks=None):
     
        if self.transaction_in_progress:
            raise RuntimeError("���������� �������� ����� �� ����� ����������.")

        allocated = []
        if preferred_blocks:
            for block in preferred_blocks:
                if block in self.free_blocks:
                    self.free_blocks.remove(block)
                    allocated.append(block)
                    if len(allocated) == count:
                        break

        while len(allocated) < count:
            if not self.free_blocks:
                raise RuntimeError("������������ ��������� ������ ��� ���������.")
            allocated.append(self.free_blocks.pop())

        return allocated

    def free_blocks(self, blocks):
        
        if self.transaction_in_progress:
            raise RuntimeError("���������� ���������� ����� �� ����� ����������.")

        for block in blocks:
            self.free_blocks.add(block)

    def write_data(self, data, blocks=None):
        if self.transaction_in_progress:
            raise RuntimeError("���������� ���������� ������ �� ����� ����������.")

        if blocks is None:
            blocks = self.allocate_blocks(len(data) // self.block_size + 1)

        for i, block in enumerate(blocks):
            with open(self.file_path, "r+b") as f:
                f.seek(block * self.block_size)
                f.write(data[i * self.block_size: (i + 1) * self.block_size])

        return blocks


    def read_data(self, blocks):
        data = b""
        with open(self.file_path, "rb") as f:
            for block in blocks:
                f.seek(block * self.block_size)
                data += f.read(self.block_size)
        return data

    def get_info(self):
        info = {
            'block_size': self.block_size,
            'num_blocks': self.num_blocks,
            'free_blocks_count': len(self.free_blocks),
            'used_blocks_count': self.num_blocks - len(self.free_blocks),
            'service_info_size': self.block_size * len(self.free_blocks), 
            'transaction_cache_size': len(self.transaction_cache) * self.block_size,
        }
        return info

    def start_transaction(self):
        if self.transaction_in_progress:
            raise RuntimeError("���������� ��� ������.")
        self.transaction_in_progress = True
        self.transaction_cache = []  

    def commit_transaction(self):
        if not self.transaction_in_progress:
            raise RuntimeError("��� �������� ����������.")
        self.transaction_in_progress = False
        with open(self.file_path, "r+b") as f:
            for block, data in self.transaction_cache:
                f.seek(block * self.block_size)
                f.write(data)
        self.transaction_cache.clear()  

    def rollback_transaction(self):
        if not self.transaction_in_progress:
            raise RuntimeError("��� �������� ����������.")
        self.transaction_in_progress = False
        self.transaction_cache.clear()  

    def add_to_transaction_cache(self, block, data):
        if not self.transaction_in_progress:
            raise RuntimeError("��� �������� ����������.")
        self.transaction_cache.append((block, data))
        
file_path = r"C:\Users\��������\Desktop\obsh.txt"
block_space = BlockSpace(file_path)

block_space.initialize(block_size=4096, num_blocks=1024)  

allocated = block_space.allocate_blocks(2)
print(f"���������� �����: {allocated}")

data = b"obshaia pamattttttttttttttttt" + b"\x00" * (4096 - len(b"obshaia pamattttttttttttttttt"))  
blocks = block_space.write_data(data, blocks=allocated)
print(f"������ �������� � �����: {blocks}")


read_data = block_space.read_data(blocks)
print(f"����������� ������ �� ����������: {read_data}")


block_space.start_transaction()
block_space.add_to_transaction_cache(allocated[0], b"New Data in Block" + b"\x00" * (4096 - len(b"New Data in Block")))  # ��������� �� ������� �����

# ��������� ���������� ������ ����������
info = block_space.get_info()
print(f"���������� � ������� ������������ ������ ����������: {info}")

block_space.commit_transaction()  

# ������ ������ ����� ����������
read_data = block_space.read_data(blocks)
print(f"����������� ������ ����� ����������: {read_data}")

info = block_space.get_info()
print(f"���������� � ������� ������������: {info}")

# �������� ������ ����������
block_space.start_transaction()
block_space.add_to_transaction_cache(allocated[0], b"Rollback Data" + b"\x00" * (4096 - len(b"Rollback Data")))

# ��������� ���������� ������ ����������
info = block_space.get_info()
print(f"���������� � ������� ������������ ������ ���������� (�����): {info}")

block_space.rollback_transaction()

info = block_space.get_info()
print(f"���������� � ������� ������������: {info}")

# ������ ������ ����� ������
read_data = block_space.read_data(blocks)
print(f"����������� ������ ����� ������ ����������: {read_data}")