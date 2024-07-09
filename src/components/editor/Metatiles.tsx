import "./Metatiles.scss";
import { MouseEvent } from "react";

interface MetatilesProps {
  metatiles: any;
}
export default function Metatiles({ metatiles }: MetatilesProps) {
  const handleMetatileClick = (event: MouseEvent<HTMLElement>) => {
    // get metatile id
    const id: string | null = event.currentTarget.getAttribute("data-id");

    // use ID to get info about the tile

    // open the modal
    // populate modal with metatile data, by updating redux state
  };
  return (
    <div className="metatiles">
      {Object.keys(metatiles).map((key, index) => (
        <div
          data-id={index}
          onClick={handleMetatileClick}
          className="metatile"
          key={key}
        >
          <img
            className="metatile"
            key={"metatile-" + key}
            src={`data:image/png;base64,${metatiles[key]}`}
          />
          <span>{index}</span>
        </div>
      ))}
    </div>
  );
}
