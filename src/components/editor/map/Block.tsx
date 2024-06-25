import "./Block.scss";

export default function Block({
  id,
  row,
  col,
  metatileImage,
}: {
  id: number;
  row: number;
  col: number;
  metatileImage: string;
}) {
  return (
    <div className="block" key={"block-" + id} data-row={row} data-col={col}>
      <img src={`data:image/png;base64,${metatileImage}`} />
      <span>{id}</span>
    </div>
  );
}
