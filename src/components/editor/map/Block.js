export default function Block(
  { id }: { id: number }
) {
  return (
    <div
      className="block"
      style={{
        border: '1px solid black',
        display: 'flex',
        justifyContent: 'center',
        alignItems: 'center',
        width: '33px',
        height: '33px',
      }}
    >
      {id}
    </div>
  );
}
