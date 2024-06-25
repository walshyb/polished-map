import "./Metatiles.scss";

interface MetatilesProps {
  metatiles: any;
}
export default function Metatiles({ metatiles }: MetatilesProps) {
  return (
    <div className="metatiles">
      {Object.keys(metatiles).map((key) => (
        <div key={key}>
          <img
            className="metatile"
            key={"metatile-" + key}
            src={`data:image/png;base64,${metatiles[key]}`}
          />
        </div>
      ))}
    </div>
  );
}
