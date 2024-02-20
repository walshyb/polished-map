interface Size {
  width: number;
  height: number;
}
 export function calculateMapSizes(size: number): Size {
  // Idk, we might want to save this
  const validSizes: Size[] = [];

  for (let width = size; width >= 1; width--) {
    const height = Math.floor(size / width);

    if (width * height == size) {
      validSizes.push({ width, height });
    }
  }

  // Prefer size multiples of 10x9, the typical unit size
  return validSizes[Math.floor((validSizes.length - 1) / 2)];
}
