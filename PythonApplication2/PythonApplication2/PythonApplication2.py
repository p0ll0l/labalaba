
import os
from datetime import datetime
import fnmatch
import shutil


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
        self.used_blocks = set()  # ������� ��������� �������������� ������

        try:
            with open(self.file_path, "wb") as f:
                f.write(b"\x00" * (block_size * num_blocks))
            self.free_blocks = set(range(num_blocks))
            total_size_mb = (self.num_blocks * self.block_size) / (1024 * 1024)
            print(f"������������� �������:")
            print(f"- {self.num_blocks} ������ �� {self.block_size} ����")
            print(f"- ����� ������: {total_size_mb:.2f} ��")
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

    # � ������ BlockSpace ������������ ����� free_blocks � free_blocks_space
    def free_blocks_space(self, blocks_to_free):  # ���� free_blocks
        if self.transaction_in_progress:
            raise RuntimeError("���������� ���������� ����� �� ����� ����������.")

        for block in blocks_to_free:
            if block in self.used_blocks:
                self.used_blocks.remove(block)
                self.free_blocks.add(block)
                print(f"���������� ���� {block}")
            else:
                print(f"��������������: ������� ���������� �������������� ���� {block}")

    def write_data(self, data, blocks):
        if not blocks:
            raise ValueError("�� ������� ����� ��� ������")

        try:
            with open(self.file_path, "r+b") as f:
                for i, block in enumerate(blocks):
                    f.seek(block * self.block_size)
                    block_data = data[i * self.block_size:(i + 1) * self.block_size]
                    block_data = block_data.ljust(self.block_size, b'\x00')
                    f.write(block_data)
                    self.used_blocks.add(block)
                    if block in self.free_blocks:
                        self.free_blocks.remove(block)

            print(f"�������� {len(data)} ���� � ����� {blocks}")
            return True
        except Exception as e:
            print(f"������ ��� ������ ������: {e}")
            return False

    def read_data(self, blocks, size=None):
        data = b""
        try:
            with open(self.file_path, "rb") as f:
                for block in blocks:
                    f.seek(block * self.block_size)
                    block_data = f.read(self.block_size)
                    data += block_data
            if size is not None:
                data = data[:size]  # �������� �� ������� �������
            print(f"��������� {len(data)} ����")
            return data
        except Exception as e:
            print(f"������ ��� ������ ������: {e}")
            return b""

    def get_info(self):
        info = {
            'block_size': self.block_size,
            'num_blocks': self.num_blocks,
            'free_blocks_count': len(self.free_blocks),
            'used_blocks_count': len(self.used_blocks),
            'free_space': len(self.free_blocks) * self.block_size / 1024,  # � ��
            'used_space': len(self.used_blocks) * self.block_size / 1024,  # � ��
            'transaction_cache_size': len(self.transaction_cache)
        }
        return info


class FileEntry:
    def __init__(self, name, is_directory=False):
        self.name = name
        self.is_directory = is_directory
        self.size = 0
        self.blocks = []
        self.created_time = datetime.now()
        self.modified_time = datetime.now()
        self.current_position = 0


class Directory:
    def __init__(self, name, parent=None):
        self.name = name
        self.entries = {}
        self.parent = parent
        self.is_directory = True
        self.created_time = datetime.now()
        self.modified_time = datetime.now()

class FileSystem:
    def __init__(self, block_space):
        self.block_space = block_space
        self.root = Directory("/", None)  # ���� ��������� parent=None ��� ��������� ��������
        self.current_directory = self.root
        self.open_files = {}
        self.file_handles = {}
        self.next_handle = 1

    def create_file(self, path):
        dir_path, filename = os.path.split(self._normalize_path(path))
        if len(filename) > 256:
            raise ValueError("��� ����� �� ����� ��������� 256 ��������")

        target_dir = self._get_directory(dir_path)
        if filename in target_dir.entries:
            raise FileExistsError(f"���� {filename} ��� ����������")

        file_entry = FileEntry(filename)
        target_dir.entries[filename] = file_entry
        return file_entry

    def open_file(self, path, mode='r'):
        path = self._normalize_path(path)
        dir_path, filename = os.path.split(path)

        parent_dir = self._get_directory(dir_path)

        if mode == 'w' and filename not in parent_dir.entries:
            file_entry = self.create_file(path)
        elif filename not in parent_dir.entries:
            raise FileNotFoundError(f"���� {filename} �� ������")
        else:
            file_entry = parent_dir.entries[filename]

        if file_entry.is_directory:
            raise IsADirectoryError(f"{filename} �������� ���������")

        handle = self.next_handle
        self.next_handle += 1
        self.open_files[handle] = file_entry
        self.file_handles[handle] = {'mode': mode, 'position': 0}  # �������������� ���������� � �������� �����������

        print(f"������ ����: {path}, �����: {mode}, ����������: {handle}")
        return handle

    def seek(self, handle, position):
        if handle not in self.file_handles:
            raise ValueError("�������� ���������� �����")

        file_entry = self.open_files[handle]
        if position < 0 or position > file_entry.size:
            raise ValueError("������������ �������")

        self.file_handles[handle]['position'] = position
        print(f"����������� ������� {position} ��� �����")

    def read(self, handle, length):
        if handle not in self.open_files:
            raise ValueError("�������� ���������� �����")

        if handle not in self.file_handles:
            raise ValueError("���� �� ������ ���������")

        if self.file_handles[handle]['mode'] != 'r':
            raise ValueError("���� �� ������ ��� ������")

        file_entry = self.open_files[handle]
        position = self.file_handles[handle]['position']

        if position >= file_entry.size:
            return b""

        # ������ ������ ������ ���������� ������
        data = self.block_space.read_data(file_entry.blocks, file_entry.size)
        result = data[position:position + length]

        self.file_handles[handle]['position'] += len(result)
        return result

    def write(self, file_handle, data):
        if file_handle not in self.open_files:
            raise ValueError("���� �� ������")

        file_entry = self.open_files[file_handle]
        if len(data) + file_entry.current_position > 2 ** 32 - 1:
            raise ValueError("�������� ������������ ������ �����")

        blocks_needed = (len(data) + self.block_space.block_size - 1) // self.block_space.block_size
        new_blocks = self.block_space.allocate_blocks(blocks_needed)

        # ��������� �������� ������
        try:
            self.block_space.write_data(data, new_blocks)
            file_entry.blocks.extend(new_blocks)
            file_entry.size = file_entry.current_position + len(data)
            file_entry.modified_time = datetime.now()
            print(f"�������� {len(data)} ����")
        except Exception as e:
            self.block_space.free_blocks(new_blocks)
            raise e

    def close_file(self, handle):
        if handle in self.open_files:
            del self.open_files[handle]
        if handle in self.file_handles:
            del self.file_handles[handle]
        print(f"���� ������, ����������: {handle}")

    def delete_file(self, path):
        path = self._normalize_path(path)
        dir_path, filename = os.path.split(path)
        target_dir = self._get_directory(dir_path)

        if filename not in target_dir.entries:
            raise FileNotFoundError(f"���� {filename} �� ������")

        file_entry = target_dir.entries[filename]
        if file_entry.is_directory:
            raise IsADirectoryError(f"{filename} �������� ���������")

        if path in self.open_files.values():
            raise RuntimeError("������ ������� �������� ����")

        # ����������� �����
        self.block_space.free_blocks_space(file_entry.blocks)  # ���������� ����� ��� ������
        del target_dir.entries[filename]
        print(f"���� {filename} ������� ������")

    def create_directory(self, path):
        path = self._normalize_path(path)
        dir_path, dirname = os.path.split(path)

        if len(dirname) > 256:
            raise ValueError("��� �������� �� ����� ��������� 256 ��������")

        parent_dir = self._get_directory(dir_path)
        if dirname in parent_dir.entries:
            raise FileExistsError(f"������� {dirname} ��� ����������")

        new_dir = Directory(dirname, parent_dir)  # �������� parent ��� ��������
        parent_dir.entries[dirname] = new_dir
        print(f"������ �������: {path}")

    def delete_directory(self, path):
        path = self._normalize_path(path)
        dir_path, dirname = os.path.split(path)

        parent_dir = self._get_directory(dir_path)
        if dirname not in parent_dir.entries:
            raise FileNotFoundError(f"������� {dirname} �� ������")

        dir_entry = parent_dir.entries[dirname]
        if not dir_entry.is_directory:
            raise NotADirectoryError(f"{dirname} �� �������� ���������")

        if dir_entry.entries:
            raise OSError(f"������� {dirname} �� ����")

        del parent_dir.entries[dirname]

    def change_directory(self, path):
        path = self._normalize_path(path)
        new_dir = self._get_directory(path)
        self.current_directory = new_dir

    def get_current_directory_info(self):
        entries = []
        current_dir = self.current_directory
        for name, entry in current_dir.entries.items():
            entry_info = {
                'name': name,
                'is_directory': entry.is_directory,
                'size': getattr(entry, 'size', 0),
                'created_time': entry.created_time,
                'modified_time': entry.modified_time
            }
            entries.append(entry_info)

        return {
            'current_path': self._get_current_path(),
            'entries': sorted(entries, key=lambda x: (-x['is_directory'], x['name']))
        }

    def search_files(self, pattern="*.*"):
        results = []
        self._search_recursive(self.current_directory, pattern, results)
        return results

    def _search_recursive(self, directory, pattern, results):
        for name, entry in directory.entries.items():
            if fnmatch.fnmatch(name, pattern):
                results.append(name)
            if entry.is_directory:
                self._search_recursive(entry, pattern, results)

    def import_file(self, external_path, internal_path):
        if not os.path.exists(external_path):
            raise FileNotFoundError(f"������� ���� {external_path} �� ������")

        file_size = os.path.getsize(external_path)
        if file_size > 2 ** 32 - 1:
            raise ValueError("���� ������� �������")

        internal_handle = self.open_file(internal_path, 'w')

        with open(external_path, 'rb') as external_file:
            while True:
                chunk = external_file.read(self.block_space.block_size)
                if not chunk:
                    break
                self.write(internal_handle, chunk)

        self.close_file(internal_handle)

    def _normalize_path(self, path):
        if not path:
            return "/"

        if not path.startswith("/"):
            current_path = self._get_current_path()
            path = os.path.join(current_path, path)

        return os.path.normpath(path).replace("\\", "/")

    def _get_current_path(self):
        if self.current_directory == self.root:
            return "/"

        path_parts = []
        current = self.current_directory

        while current != self.root:
            path_parts.append(current.name)
            current = current.parent

        return "/" + "/".join(reversed(path_parts))

    def _get_directory(self, path):
        if not path or path == "/":
            return self.root

        current = self.root
        parts = [p for p in path.split("/") if p]

        for part in parts:
            if part not in current.entries:
                raise FileNotFoundError(f"������� {path} �� ������")

            entry = current.entries[part]
            if not entry.is_directory:
                raise NotADirectoryError(f"{part} �� �������� ���������")

            current = entry

        return current

# ������ �������������:
if __name__ == "__main__":
    file_path = "filesystem.dat"
    block_space = BlockSpace(file_path)
    block_space.initialize(block_size=4096, num_blocks=1024)

    fs = FileSystem(block_space)

    print("\n=== ������������ �������� ������� ===")

    # ������� ��������� ���������
    print("\n1. �������� ��������� ���������:")
    fs.create_directory("/documents")
    fs.create_directory("/documents/work")

    # ������� � ���������� ����
    print("\n2. �������� � ������ � ����:")
    file_handle = fs.open_file("/documents/test.txt", "w")
    fs.write(file_handle, b"Hello, World!")
    fs.close_file(file_handle)

    # ������ ����
    print("\n3. ������ �����:")
    file_handle = fs.open_file("/documents/test.txt", "r")
    data = fs.read(file_handle, 100)
    print(f"   ���������: '{data.decode().strip()}'")
    fs.close_file(file_handle)

    # �������� ���������� � �������� documents
    print("\n4. ���������� � �������� /documents:")
    fs.change_directory("/documents")
    dir_info = fs.get_current_directory_info()
    print(f"   ������� ����: {dir_info['current_path']}")
    print("   ����������:")
    for entry in dir_info['entries']:
        entry_type = "�������" if entry['is_directory'] else "����"
        size = entry['size']
        print(f"   - {entry_type}: {entry['name']}, ������: {size} ����")

    # ���������� �������� ������������
    print("\n5. ���������� �������� ������������:")
    space_info = block_space.get_info()
    print(f"   - ������ �����: {space_info['block_size']} ����")
    print(f"   - ����� ������: {space_info['num_blocks']}")
    print(f"   - ��������� ������: {space_info['free_blocks_count']} ({space_info['free_space']:.2f} ��)")
    print(f"   - �������������� ������: {space_info['used_blocks_count']} ({space_info['used_space']:.2f} ��)")