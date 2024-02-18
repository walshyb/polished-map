class Block {
private:
	uint8_t _row, _col;
	uint8_t _id;
public:
	Block(uint8_t row = 0, uint8_t col = 0, uint8_t id = 0);
	inline uint8_t row(void) const { return _row; }
	inline uint8_t col(void) const { return _col; }
	inline void coords(uint8_t row, uint8_t col) { _row = row; _col = col; }
	inline uint8_t id(void) const { return _id; }
	inline void id(uint8_t id) { _id = id; update_label(); }
};

