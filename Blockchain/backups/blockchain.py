from hashlib import sha256
import time
import jsonpickle


class Block:
    def __init__(self, index, data, previous_hash):
        self.index = index
        self.data = data
        self.timestamp = time.ctime(time.time())
        self.previous_hash = previous_hash
        self.nonce = 0
        self.hash = sha256(jsonpickle.encode(data, unpicklable=True).encode()).hexdigest()

    def compute_hash(self):
        return sha256(jsonpickle.encode(self, unpicklable=True).encode()).hexdigest()

    def to_string(self):
        return f"index: {self.index}\ndata: {self.data.to_string()}\ntimestamp: {self.timestamp}\n" \
               f"hash: {self.hash}\nprevious_hash: {self.previous_hash}\nnonce: {self.nonce}\n"


class Blockchain:
    difficulty = 1

    def __init__(self):
        self.chain = []
        self.create_genesis_block()

    def create_genesis_block(self):
        genesis_block = Block(-1, '', '0x0000000000000000000000000000000000000000000000000000000000000000')
        self.proof_of_work(genesis_block)
        self.chain.append(genesis_block)

    @property
    def last_block(self):
        return self.chain[-1]

    @staticmethod
    def proof_of_work(input_block):
        input_block.nonce = 0
        block_hash = input_block.hash
        computed_hash = block_hash

        while not computed_hash.startswith('0' * Blockchain.difficulty):
            input_block.nonce += 1
            computed_hash = input_block.compute_hash()

        input_block.hash = '0x' + computed_hash

    def add_to_chain(self, input_block):
        self.proof_of_work(input_block)
        self.chain.append(input_block)

    def print_blockchain(self):
        for block in self.chain[1:]:
            print(block.to_string())
