interface MetatilesProps {
  metatiles: any;
}
export default function Metatiles({ metatiles }: MetatilesProps) {
  return (
    <div>
      {Object.keys(metatiles).map((key) => (
        <div key={key}>
          <img key={key} src={`data:image/png;base64,${metatiles[key]}`} />
        </div>
      ))}
    </div>
  );
}
