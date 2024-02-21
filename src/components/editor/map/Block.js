export default function Block(
  { id, row, col }: { id: number, row: number, col: number }
) {
  return (
    <div
      className="block"
      style={{
        border: '1px solid black',
        display: 'flex',
        justifyContent: 'center',
        alignItems: 'center',
        width: '35px',
        height: '35px',
      }}
      row={row}
      col={col}
    >
      {id}
    </div>
  );
}
